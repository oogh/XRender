#
#  Be sure to run `pod spec lint XMedia.podspec" to ensure this is a
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
    corerender.exclude_files = "corerender/Android"
    
    corerender.public_header_files = "corerender/**/*.{h,hpp}", "corerender/iOS/**/*.{h,hpp}", "corerender/iOS/*.{h,hpp}"
    
    corerender.frameworks = "GLKit"
    corerender.libraries = "c++"
  end
end
