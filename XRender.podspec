#
#  Be sure to run `pod spec lint XRender.podspec" to ensure this is a
#  valid spec and to remove all comments including this before submitting the spec.
#
#  To learn more about Podspec attributes see https://guides.cocoapods.org/syntax/podspec.html
#  To see working Podspecs in the CocoaPods repo see https://github.com/CocoaPods/Specs/
#

Pod::Spec.new do |spec|
  spec.name         = "XRender"
  spec.version      = "0.0.1"
  spec.summary      = "基于OpenGL的跨平台渲染"
  
  spec.description  = <<-DESC
  基于OpenGL的跨平台渲染
  DESC
  
  spec.ios.deployment_target = "9.0"
  
  spec.homepage     = "https://github.com/oogh/XRender"
  spec.author       = { "oogh" => "oogh216@163.com" }
  spec.source       = { :git => "https://github.com/oogh/XRender.git", :tag => "#{spec.version}" }
  
  spec.subspec 'corerender' do |corerender|
    corerender.source_files  = "corerender/**/*.{h,hpp,cpp}", "corerender/iOS/**/*.{h,hpp,cpp,m,mm}"
    corerender.exclude_files = "corerender/Android", "corerender/Mac"
    corerender.public_header_files = "corerender/**/*.{h,hpp}", "corerender/iOS/**/*.{h,hpp}", "corerender/iOS/*.{h,hpp}"
    corerender.frameworks = "GLKit", "VideoToolBox", "CoreMedia", "OpenGLES"
    corerender.libraries = "c++", "iconv"
    
    corerender.subspec 'ffmpeg' do |ffmpeg|
       ffmpeg.xcconfig = {"HEADER_SEARCH_PATHS" => '"$(PODS_TARGET_SRCROOT)/3rdparty/iOS/ffmpeg/include"'}
       ffmpeg.vendored_libraries  = '3rdparty/iOS/ffmpeg/lib/*.a'
       ffmpeg.libraries = 'z', 'bz2'
    end
    
    corerender.subspec 'fdkaac' do |fdkaac|
       fdkaac.xcconfig = {"HEADER_SEARCH_PATHS" => '"$(PODS_TARGET_SRCROOT)/3rdparty/iOS/fdkaac/include"'}
       fdkaac.vendored_libraries  = '3rdparty/iOS/fdkaac/lib/*.a'
    end
    
    corerender.subspec 'lame' do |lame|
       lame.xcconfig = {"HEADER_SEARCH_PATHS" => '"$(PODS_TARGET_SRCROOT)/3rdparty/iOS/lame/include"'}
       lame.vendored_libraries  = '3rdparty/iOS/lame/lib/*.a'
    end
    
    corerender.subspec 'x264' do |x264|
       x264.xcconfig = {"HEADER_SEARCH_PATHS" => '"$(PODS_TARGET_SRCROOT)/3rdparty/iOS/x264/include"'}
       x264.vendored_libraries  = '3rdparty/iOS/x264/lib/*.a'
    end
    
    corerender.subspec 'yuv' do |yuv|
       yuv.xcconfig = {"HEADER_SEARCH_PATHS" => '"$(PODS_TARGET_SRCROOT)/3rdparty/iOS/libyuv/include"'}
       yuv.vendored_libraries  = '3rdparty/iOS/libyuv/lib/*.a'
    end
    
    corerender.subspec 'sox' do |sox|
       sox.xcconfig = {"HEADER_SEARCH_PATHS" => '"$(PODS_TARGET_SRCROOT)/3rdparty/iOS/sox/include"'}
       sox.vendored_libraries  = '3rdparty/iOS/sox/lib/*.a'
    end
  end
end
