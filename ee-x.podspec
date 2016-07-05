Pod::Spec.new do |s|
    s.name              = "ee-x"
    s.version           = "0.0.1"
    s.summary           = "ee-x"
    s.description       = "ee-x"
  
    s.homepage          = "https://github.com/enrevol/ee-x"
  
    # s.license         = { :type => "MIT", :file => "FILE_LICENSE" }
    s.author            = "Hoang Hai"
  
    s.ios.deployment_target = "5.0"
    s.osx.deployment_target = "10.7"
  
    s.source            = { :git => "https://github.com/enrevol/ee-x.git", :commit => "51795226bd19d1380e57257242228ff3eafada2a" }
  
    s.source_files      = "core/libraries/**/*.{h,hpp,cpp}", 
                          "core/src/**/*.{hpp,cpp,mm}", 
                          "core/proj.ios_mac/src/**/*.{h,m,mm}",
                          "crashlytics/src/**/*.{hpp,cpp,mm}",
                          "crashlytics/proj.ios_mac/src/**/*.{h,m,mm}"

    s.exclude_files     = "core/libraries/akrzemi1-optional/*.cpp",
                          "core/src/Jni*",
                          "core/src/*-Android*"
  
    s.public_header_files = "crashlytics/src/CrashlyticsProtocol.hpp"
  
    s.ios.frameworks  = "Foundation"

    s.requires_arc = false
  
    s.xcconfig = { "HEADER_SEARCH_PATHS" => "../libraries" }

    s.dependency 'Crashlytics', '~> 3.7'
    s.dependency 'Fabric', '~> 1.6' 
end
