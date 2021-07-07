#!/bin/bash

# get into the script's folder
cd "$(dirname "$0")"
cd ../../..

# library makefile data
cc="gcc"

src+=("res/willis/src/willis.c")
src+=("res/willis/src/debug.c")
src+=("res/willis/src/x11.c")
src+=("res/willis/src/xkb.c")
src+=("res/cursoryx/src/x11.c")
src+=("res/dpishit/src/x11.c")
src+=("res/dpishit/src/nix.c")
src+=("res/razorbeard/src/razorbeard.c")
src+=("res/razorbeard/src/default_widgets/razorbeard_math.c")
src+=("res/razorbeard/src/default_widgets/razorbeard_helpers.c")
src+=("res/razorbeard/src/default_widgets/razorbeard_default_widgets.c")
src+=("example/helpers.c")

flags+=("-std=c99" "-pedantic")
flags+=("-Wall" "-Wextra" "-Werror=vla" "-Werror")
flags+=("-Wno-address-of-packed-member")
flags+=("-Wno-unused-parameter")
flags+=("-Ires/globox/include")
flags+=("-Ires/willis/src")
flags+=("-Ires/cursoryx/src")
flags+=("-Ires/dpishit/src")
flags+=("-Ires/razorbeard/src")
flags+=("-Ires/razorbeard/src/default_widgets")
flags+=("-Iexample")

defines+=("-DGLOBOX_PLATFORM_X11")
defines+=("-DWILLIS_X11")
defines+=("-DWILLIS_DEBUG")
defines+=("-DCURSORYX_X11")
defines+=("-DDPISHIT_X11")

# generated linker arguments
link+=("xcb")
link+=("xcb-cursor")
link+=("xcb-randr")
link+=("xcb-xfixes")
link+=("xcb-xinput")
link+=("xcb-xkb")
link+=("xcb-xrm")
link+=("xkbcommon")
link+=("xkbcommon-x11")

# context type
read -p "select context type ([1] software | [2] egl | [3] glx): " context

case $context in
	[1]* ) # software context
makefile=makefile_example_x11_software
name="example_x11_software"
globox="globox_x11_software"
src+=("example/software.c")
defines+=("-DGLOBOX_CONTEXT_SOFTWARE")
link+=("xcb-shm")
link+=("xcb-render")
	;;

	[2]* ) # egl context
makefile=makefile_example_x11_egl
name="example_x11_egl"
globox="globox_x11_egl"
src+=("example/egl.c")
defines+=("-DGLOBOX_CONTEXT_EGL")
link+=("egl")
link+=("glesv2")
	;;
esac

# link type
read -p "select library type ([1] static | [2] shared): " library

case $library in
	[1]* ) # link statically
obj+=("res/globox/lib/globox/x11/$globox.a")
cmd="./$name"
	;;

	[2]* ) # link dynamically
ldflags+=("-Lres/globox/lib/globox/x11")
ldlibs+=("-l:$globox.so")
cmd="LD_LIBRARY_PATH=../res/globox/lib/globox/x11 ./$name"
	;;
esac

# default target
default+=("res/globox")
default+=("bin/\$(NAME)")

# valgrind flags
valgrind+=("--show-error-list=yes")
valgrind+=("--show-leak-kinds=all")
valgrind+=("--track-origins=yes")
valgrind+=("--leak-check=full")
valgrind+=("--suppressions=../res/valgrind.supp")

# makefile start
echo ".POSIX:" > $makefile
echo "NAME = $name" >> $makefile
echo "CMD = $cmd" >> $makefile
echo "CC = $cc" >> $makefile

# makefile linking info
echo "" >> $makefile
for flag in $(pkg-config ${link[@]} --cflags) ${ldflags[@]}; do
	echo "LDFLAGS+= $flag" >> $makefile
done

echo "" >> $makefile
for flag in $(pkg-config ${link[@]} --libs) ${ldlibs[@]}; do
	echo "LDLIBS+= $flag" >> $makefile
done

# makefile compiler flags
echo "" >> $makefile
for flag in ${flags[@]}; do
	echo "CFLAGS+= $flag" >> $makefile
done

echo "" >> $makefile
for define in ${defines[@]}; do
	echo "CFLAGS+= $define" >> $makefile
done

# makefile object list
echo "" >> $makefile
for file in ${src[@]}; do
	folder=$(dirname "$file")
	filename=$(basename "$file" .c)
	echo "OBJ+= $folder/$filename.o" >> $makefile
done

echo "" >> $makefile
for prebuilt in ${obj[@]}; do
	echo "OBJ_EXTRA+= $prebuilt" >> $makefile
done

# generate valgrind flags
echo "" >> $makefile
for flag in ${valgrind[@]}; do
	echo "VALGRIND+= $flag" >> $makefile
done

# makefile default target
echo "" >> $makefile
echo "default: ${default[@]}" >> $makefile

# makefile linux targets
echo "" >> $makefile
cat make/example/templates/targets_linux.make >> $makefile

# makefile object targets
echo "" >> $makefile
for file in ${src[@]}; do
	$cc $defines -MM -MG $file >> $makefile
done

# makefile extra targets
echo "" >> $makefile
cat make/example/templates/targets_extra.make >> $makefile
