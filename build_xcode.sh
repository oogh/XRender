sh build_clean.sh

mkdir cmake-build-xcode

cd cmake-build-xcode

cmake -G "Xcode" ..

open XRender.xcodeproj
