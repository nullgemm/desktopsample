#!/bin/bash

# get into the script's folder
cd "$(dirname "$0")"
cd ../../..

tag=$(git tag | tail -n 1)

# library makefile data
cc="x86_64-w64-mingw32-gcc"

src+=("res/willis/src/willis.c")
src+=("res/willis/src/debug.c")
src+=("res/willis/src/windows.c")
src+=("res/cursoryx/src/windows.c")
src+=("res/dpishit/src/windows.c")

flags+=("-std=c99" "-pedantic")
flags+=("-Wall" "-Wextra" "-Werror=vla" "-Werror")
flags+=("-Wno-address-of-packed-member")
flags+=("-Wno-unused-parameter")
flags+=("-Wno-implicit-fallthrough")
flags+=("-Wno-cast-function-type")
flags+=("-Wno-incompatible-pointer-types")
flags+=("-Ires/globox/include")
flags+=("-Ires/willis/src")
flags+=("-Ires/cursoryx/src")
flags+=("-Ires/dpishit/src")

defines+=("-DGLOBOX_PLATFORM_WINDOWS")
defines+=("-DWILLIS_WINDOWS")
defines+=("-DWILLIS_DEBUG")
defines+=("-DCURSORYX_WINDOWS")
defines+=("-DDPISHIT_WINDOWS")
defines+=("-DUNICODE")
defines+=("-D_UNICODE")
defines+=("-DWINVER=0x0A00")
defines+=("-D_WIN32_WINNT=0x0A00")
defines+=("-DCINTERFACE")
defines+=("-DCOBJMACROS")

# generated linker arguments
ldflags+=("-fno-stack-protector")
ldlibs+=("-lgdi32")
ldlibs+=("-ldwmapi")
ldlibs+=("-lshcore")
ldlibs+=("-mwindows")

# context type
read -p "select context type ([1] software | [2] egl): " context

case $context in
	[1]* ) # software context
makefile=makefile_example_windows_software
name="example_windows_software"
globox="globox_windows_software"
src+=("example/software.c")
defines+=("-DGLOBOX_CONTEXT_SOFTWARE")
	;;

	[2]* ) # egl context
makefile=makefile_example_windows_egl
name="example_windows_egl"
globox="globox_windows_egl"
src+=("example/egl.c")
flags+=("-Ires/egl_headers")
defines+=("-DGLOBOX_CONTEXT_EGL")
ldflags+=("-Lres/eglproxy/lib/mingw")
ldlibs+=("-leglproxy")
ldlibs+=("-lopengl32")
default+=("res/egl_headers")
default+=("bin/eglproxy.dll")
	;;
esac

# link globox
obj+=("res/globox/lib/globox/windows/"$globox"_mingw.a")

# default target
cmd="../make/scripts/dll_copy.sh "$globox"_mingw.dll && wine ./$name.exe"
default+=("res/globox")
default+=("bin/\$(NAME)")

# makefile start
echo ".POSIX:" > $makefile
echo "NAME = $name" >> $makefile
echo "CMD = $cmd" >> $makefile
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

# makefile default target
echo "" >> $makefile
echo "default: ${default[@]}" >> $makefile

# makefile linux targets
echo "" >> $makefile
cat make/example/templates/targets_windows_mingw.make >> $makefile

# makefile object targets
echo "" >> $makefile
for file in ${src[@]}; do
	$cc $defines -MM -MG $file >> $makefile
done

# makefile extra targets
echo "" >> $makefile
cat make/example/templates/targets_extra.make >> $makefile
