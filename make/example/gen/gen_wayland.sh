#!/bin/bash

# get into the script's folder
cd "$(dirname "$0")"
cd ../../..

# library makefile data
cc="gcc"

src+=("res/willis/src/willis.c")
src+=("res/willis/src/debug.c")
src+=("res/willis/src/wayland.c")
src+=("res/willis/src/xkb.c")
src+=("res/cursoryx/src/wayland.c")
src+=("res/dpishit/src/wayland.c")
src+=("res/dpishit/src/nix.c")
src+=("res/razorbeard/src/razorbeard.c")
src+=("res/razorbeard/src/default_widgets/razorbeard_math.c")
src+=("res/razorbeard/src/default_widgets/razorbeard_helpers.c")
src+=("res/razorbeard/src/default_widgets/razorbeard_default_widgets.c")
src+=("res/razorbeard/src/default_widgets/button/razorbeard_widget_button.c")
src+=("res/razorbeard/src/default_widgets/checkbox/razorbeard_widget_checkbox.c")
src+=("res/razorbeard/src/default_widgets/dropmenu/razorbeard_widget_dropmenu.c")
src+=("res/razorbeard/src/default_widgets/frame/razorbeard_widget_frame.c")
src+=("res/razorbeard/src/default_widgets/handles/razorbeard_widget_handles.c")
src+=("res/razorbeard/src/default_widgets/image/razorbeard_widget_image.c")
src+=("res/razorbeard/src/default_widgets/numberbox/razorbeard_widget_numberbox.c")
src+=("res/razorbeard/src/default_widgets/pager/razorbeard_widget_pager.c")
src+=("res/razorbeard/src/default_widgets/popup/razorbeard_widget_popup.c")
src+=("res/razorbeard/src/default_widgets/progressbar/razorbeard_widget_progressbar.c")
src+=("res/razorbeard/src/default_widgets/radiobutton/razorbeard_widget_radiobutton.c")
src+=("res/razorbeard/src/default_widgets/scrollbar/razorbeard_widget_scrollbar.c")
src+=("res/razorbeard/src/default_widgets/separator/razorbeard_widget_separator.c")
src+=("res/razorbeard/src/default_widgets/slider/razorbeard_widget_slider.c")
src+=("res/razorbeard/src/default_widgets/tabs/razorbeard_widget_tabs.c")
src+=("res/razorbeard/src/default_widgets/text/razorbeard_widget_text.c")
src+=("res/razorbeard/src/default_widgets/textarea/razorbeard_widget_textarea.c")
src+=("res/razorbeard/src/default_widgets/textbox/razorbeard_widget_textbox.c")
src+=("example/helpers.c")

flags+=("-std=c99" "-pedantic")
flags+=("-Wall" "-Wextra" "-Werror=vla" "-Werror")
flags+=("-Wno-address-of-packed-member")
flags+=("-Wno-unused-parameter")
flags+=("-Ires/wayland_headers")
flags+=("-Ires/globox/include")
flags+=("-Ires/willis/src")
flags+=("-Ires/cursoryx/src")
flags+=("-Ires/dpishit/src")
flags+=("-Ires/razorbeard/src")
flags+=("-Ires/razorbeard/src/default_widgets")
flags+=("-Ires/razorbeard/src/default_widgets/*")
flags+=("-Iexample")

defines+=("-DGLOBOX_PLATFORM_WAYLAND")
defines+=("-DWILLIS_WAYLAND")
defines+=("-DWILLIS_DEBUG")
defines+=("-DCURSORYX_WAYLAND")
defines+=("-DDPISHIT_WAYLAND")

# generated linker arguments
link+=("wayland-client")
link+=("wayland-cursor")
link+=("xkbcommon")
ldlibs+=("-lrt")

# context type
read -p "select context type ([1] software | [2] egl): " context

case $context in
	[1]* ) # software context
makefile=makefile_example_wayland_software
name="example_wayland_software"
globox="globox_wayland_software"
src+=("example/software.c")
defines+=("-DGLOBOX_CONTEXT_SOFTWARE")
	;;

	[2]* ) # egl context
makefile=makefile_example_wayland_egl
name="example_wayland_egl"
globox="globox_wayland_egl"
src+=("example/egl.c")
defines+=("-DGLOBOX_CONTEXT_EGL")
link+=("wayland-egl")
link+=("egl")
link+=("glesv2")
	;;
esac

# link type
read -p "select library type ([1] static | [2] shared): " library

case $library in
	[1]* ) # link statically
obj+=("res/globox/lib/globox/wayland/$globox.a")
cmd="./$name"
	;;

	[2]* ) # link dynamically
ldflags+=("-Lres/globox/lib/globox/wayland")
ldlibs+=("-l:$globox.so")
cmd="LD_LIBRARY_PATH=../res/globox/lib/globox/wayland ./$name"
	;;
esac

# default target
default+=("res/globox")
default+=("res/wayland_headers")
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
