PROJECT=./examples/Android/XRender
APP=$PROJECT/app
CORERENDER=$PROJECT/corerender

rm -rf $PROJECT/.gradle
rm -rf $PROJECT/.idea
rm $PROJECT/*.iml

rm -rf $APP/build
rm $APP/*.iml

rm -rf $CORERENDER/.cxx
rm -rf $CORERENDER/build
rm $CORERENDER/*.iml
