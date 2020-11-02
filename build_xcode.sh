sh build_clean.sh

mkdir cmake-build-xcode

cd cmake-build-xcode

cmake -S. -B_builds -GXcode \
    "-DCMAKE_OSX_ARCHITECTURES=x86_64" \
    -DCMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH=NO  ..  || exit 1

if [ -f XRender.xcodeproj ]; then
    open XRender.xcodeproj
fi
