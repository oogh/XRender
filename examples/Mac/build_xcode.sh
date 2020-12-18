sh ../../build_clean.sh

mkdir cmake-build-xcode

cd cmake-build-xcode

cmake -S. -GXcode \
    "-DCMAKE_OSX_ARCHITECTURES=x86_64" \
    -DCMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH=NO .. || exit 1

if [ -d XRender.xcodeproj ]; then
    open XRender.xcodeproj
fi
