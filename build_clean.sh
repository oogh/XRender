# andorid example clean
PROJ_ANDROID=./examples/Android/XRender
APP=$PROJ_ANDROID/app
CORERENDER=$PROJ_ANDROID/corerender

echo "[CLEAN] Begin!"

if [ -d "$PROJ_ANDROID/.gradle" ]; then
    rm -rf $PROJ_ANDROID/.gradle
    echo "[CLEANing] $PROJ_ANDROID/.gradle"
fi

if [ -d "$PROJ_ANDROID/.idea" ]; then
    rm -rf $PROJ_ANDROID/.idea
    echo "[CLEANing] $PROJ_ANDROID/.idea"
fi

if [ -f "$PROJ_ANDROID/*.iml" ]; then
    rm -rf $PROJ_ANDROID/*.iml
    echo "[CLEANing] $PROJ_ANDROID/*.iml"
fi

if [ -d "$APP/build" ]; then
    rm -rf $APP/build
    echo "[CLEANing] $APP/build"
fi

if [ -f "$APP/*.iml" ]; then
    rm $APP/*.iml
    echo "[CLEANing] $APP/*.iml"
fi

if [ -d "$CORERENDER/.cxx" ]; then
    rm -rf $CORERENDER/.cxx
    echo "[CLEANing] $CORERENDER/.cxx"
fi

if [ -d "$CORERENDER/build" ]; then
    rm -rf $CORERENDER/build
    echo "[CLEANing] $CORERENDER/build"
fi

if [ -f "$CORERENDER/*.iml" ]; then
    rm $CORERENDER/*.iml
    echo "[CLEANing] $CORERENDER/*.iml"
fi

# ios example clean
PROJ_IOS=./examples/iOS/XRender

if [ -f "$PROJ_IOS/Podfile.lock" ]; then
    rm $PROJ_IOS/Podfile.lock
    echo "[CLEANing] $PROJ_IOS/Podfile.lock"
fi

if [ -d "$PROJ_IOS/Pods" ]; then
    rm -rf $PROJ_IOS/Pods
    echo "[CLEANing] $PROJ_IOS/Pods"
fi

if [ -d "$PROJ_IOS/XRender.xcworkspace" ]; then
    rm -rf $PROJ_IOS/XRender.xcworkspace
    echo "[CLEANing] $PROJ_IOS/XRender.xcworkspace"
fi

echo "[CLEAN] Done!"
