#!/bin/bash

# get into the script's folder
cd "$(dirname "$0")"
cd ../../..

# library makefile data
src+=("res/willis/src/willis.c")
src+=("res/willis/src/debug.c")
src+=("res/willis/src/macos.c")
src+=("res/cursoryx/src/macos.c")
src+=("res/dpishit/src/macos.c")
src+=("res/razorbeard/src/razorbeard.c")
src+=("res/razorbeard/src/default_widgets/razorbeard_math.c")
src+=("res/razorbeard/src/default_widgets/razorbeard_helpers.c")
src+=("res/razorbeard/src/default_widgets/razorbeard_default_widgets.c")
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
flags+=("-Ires/globox/include")
flags+=("-Ires/willis/src")
flags+=("-Ires/cursoryx/src")
flags+=("-Ires/dpishit/src")
flags+=("-Ires/razorbeard/src")
flags+=("-Ires/razorbeard/src/default_widgets")
flags+=("-Iexample")

defines+=("-DGLOBOX_PLATFORM_MACOS")
defines+=("-DWILLIS_MACOS")
defines+=("-DWILLIS_DEBUG")
defines+=("-DCURSORYX_MACOS")
defines+=("-DDPISHIT_MACOS")

# generated linker arguments
ldlibs+=("-framework AppKit")

# context type
read -p "select context type ([1] software | [2] egl): " context

case $context in
	[1]* ) # software context
makefile=makefile_example_macos_software
name="example_macos_software"
globox="globox_macos_software"
src+=("example/software.c")
defines+=("-DGLOBOX_CONTEXT_SOFTWARE")
	;;

	[2]* ) # egl context
makefile=makefile_example_macos_egl
name="example_macos_egl"
globox="globox_macos_egl"
src+=("example/egl.c")
flags+=("-Ires/angle/include")
defines+=("-DGLOBOX_CONTEXT_EGL")
ldflags+=("-Lres/angle/libs")
ldlibs+=("-lEGL")
ldlibs+=("-lGLESv2")
default+=("res/angle/libs")
default+=("bin/libEGL.dylib")
	;;
esac

# toolchain type
read -p "select toolchain type ([1] osxcross | [2] native): " toolchain

case $toolchain in
	[1]* ) # cross-compiling
cc=o64-clang
objcopy="objcopy"
	;;

	[2]* ) # compiling from mac
makefile+="_native"
name+="_native"
globox+="_native"
cc=clang
objcopy="/usr/local/Cellar/binutils/*/bin/objcopy"
	;;
esac

read -p "select current toolchain ([1] osxcross | [2] native)" current_toolchain

case $current_toolchain in
	[1]* ) # generating from linux
cch=o64-clang
	;;

	[2]* ) # generating from mac
cch=clang
	;;
esac

# link type
read -p "select library type ([1] static | [2] shared): " library

case $library in
	[1]* ) # link statically
obj+=("res/globox/lib/globox/macos/$globox.a")
cmd="./$name"
	;;

	[2]* ) # link dynamically
ldflags+=("-Lres/globox/lib/globox/macos")
ldlibs+=("-l$globox")
cmd="../make/scripts/dylib_copy.sh "$globox" && ./$name"
	;;
esac

# default target
default+=("res/globox")
default+=("bin/\$(NAME).app")

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
echo "OBJCOPY = $objcopy" >> $makefile
echo "CC = $cc" >> $makefile

# makefile linking info
echo "" >> $makefile
for flag in ${ldflags[@]}; do
	echo "LDFLAGS+= $flag" >> $makefile
done

echo "" >> $makefile
for flag in ${ldlibs[@]}; do
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
cat make/example/templates/targets_macos.make >> $makefile

# makefile object targets
echo "" >> $makefile
for file in ${src[@]}; do
	$cch $defines -MM -MG $file >> $makefile
done

# makefile extra targets
echo "" >> $makefile
cat make/example/templates/targets_extra.make >> $makefile
