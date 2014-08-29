#
# Copyright (c) 2008-2014 the Urho3D project.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

require 'pathname'
require 'json'
if ENV['XCODE']
  require 'xcodeproj'
end

# Usage: rake sync (only intended to be used in a fork with remote 'upstream' set to urho3d/Urho3D)
desc 'Fetch and merge upstream urho3d/Urho3D to a Urho3D fork'
task :sync do
  system "cwb=`git symbolic-ref -q --short HEAD || git rev-parse --short HEAD`; export cwb && git fetch upstream && git checkout master && git pull && git merge -m 'Sync at #{Time.now.localtime}.' upstream/master && git push && git checkout $cwb"
end

# Usage: rake scaffolding dir=/path/to/new/project/root [project=your-project-name] [target=your-target-name]
desc 'Create a new project using Urho3D as external library'
task :scaffolding do
  abort 'Usage: rake scaffolding dir=/path/to/new/project/root [project=your-project-name] [target=your-target-name]' unless ENV['dir']
  abs_path = ENV['dir'][0, 1] == '/' ? ENV['dir'] : "#{Dir.pwd}/#{ENV['dir']}"
  project = ENV['project'] || 'Scaffolding'
  target = ENV['target'] || 'Main'
  scaffolding(abs_path, project, target)
  abs_path = Pathname.new(abs_path).realpath
  puts "\nNew project created in #{abs_path}\n\n"
  puts "To build the new project, you may need to first define and export either 'URHO3D_HOME' or 'CMAKE_PREFIX_PATH' environment variable"
  puts "Please see http://urho3d.github.io/documentation/_using_library.html for more detail. For example:\n\n"
  puts "$ URHO3D_HOME=#{Dir.pwd}; export URHO3D_HOME\n$ cd #{abs_path}\n$ ./cmake_gcc.sh -DURHO3D_64BIT=1 -DURHO3D_LUAJIT=1\n$ cd Build\n$ make\n\n"
end

# Usage: NOT intended to be used manually (if you insist then try: rake travis_ci)
desc 'Configure, build, and test Urho3D project'
task :travis_ci do
  if ENV['PACKAGE_UPLOAD']
    $configuration = 'Release'
    $testing = 0
  else
    $configuration = 'Debug'
    $testing = 1
  end
  if ENV['XCODE']
    # xctool or xcodebuild
    xcode_travis_ci
  else
    # GCC or Clang
    makefile_travis_ci
  end
end

# Usage: NOT intended to be used manually (if you insist then try: GIT_NAME=... GIT_EMAIL=... GH_TOKEN=... rake travis_ci_site_update)
desc 'Update site documentation to GitHub Pages'
task :travis_ci_site_update do
  # Pull or clone
  system 'cd doc-Build 2>/dev/null && git pull -q -r || git clone --depth=1 -q https://github.com/urho3d/urho3d.github.io.git doc-Build' or abort 'Failed to pull/clone'
  # Update credits from Readme.txt to about.md
  system "ruby -lne 'BEGIN { credits = false }; puts $_ if credits; credits = true if /bugfixes by:/; credits = false if /^$/' Readme.txt |ruby -i -le 'credits = STDIN.read; puts ARGF.read.gsub(/(?<=bugfixes by\n).*?(?=##)/m, credits)' doc-Build/about.md" or abort 'Failed to update credits'
  # Setup doxygen to use minimal theme
  system "ruby -i -pe 'BEGIN { a = {%q{HTML_HEADER} => %q{minimal-header.html}, %q{HTML_FOOTER} => %q{minimal-footer.html}, %q{HTML_STYLESHEET} => %q{minimal-doxygen.css}, %q{HTML_COLORSTYLE_HUE} => 200, %q{HTML_COLORSTYLE_SAT} => 0, %q{HTML_COLORSTYLE_GAMMA} => 20, %q{DOT_IMAGE_FORMAT} => %q{svg}, %q{INTERACTIVE_SVG} => %q{YES}} }; a.each {|k, v| gsub(/\#{k}\s*?=.*?\n/, %Q{\#{k} = \#{v}\n}) }' Docs/Doxyfile" or abort 'Failed to setup doxygen configuration file'
  system 'cp doc-Build/_includes/Doxygen/minimal-* Docs' or abort 'Failed to copy minimal-themed template'
  release = ENV['RELEASE_TAG'].empty? ? 'HEAD' : ENV['RELEASE_TAG'];
  unless release == 'HEAD'
    system "mkdir -p doc-Build/documentation/#{release}" or 'Failed to create directory for new document version'
    system "ruby -i -pe 'gsub(/HEAD/, %q{#{release}})' Docs/minimal-header.html" or 'Failed to update document version in YAML Front Matter block'
    append_new_release release, 'doc-Build/_data/urho3d.json' or abort 'Failed to add new release to document data file'
  end
  # Generate and sync doxygen pages
  system "cd Build && make -j$NUMJOBS doc >/dev/null 2>&1 && ruby -i -pe 'gsub(/(<\\/?h)2([^>]*?>)/, %q{\\13\\2}); gsub(/(<\\/?h)1([^>]*?>)/, %q{\\12\\2})' ../Docs/html/_*.html && rsync -a --delete ../Docs/html/ ../doc-Build/documentation/#{release}" or abort 'Failed to generate/rsync doxygen pages'
  # Supply GIT credentials and push site documentation to urho3d/urho3d.github.io.git
  system "cd doc-Build && pwd && git config user.name $GIT_NAME && git config user.email $GIT_EMAIL && git remote set-url --push origin https://$GH_TOKEN@github.com/urho3d/urho3d.github.io.git && git add -A . && ( git commit -q -m \"Travis CI: site documentation update at #{Time.now.utc}.\n\nCommit: https://github.com/$TRAVIS_REPO_SLUG/commit/$TRAVIS_COMMIT\n\nMessage: $COMMIT_MESSAGE\" || true) && git push -q >/dev/null 2>&1" or abort 'Failed to update site'
  # Automatically give instruction to do packaging when API has changed, unless the instruction is already given in this commit
  if ENV['PACKAGE_UPLOAD']
    instruction = 'skip'
  else
    instruction = 'package'
  end
  # Supply GIT credentials and push API documentation to urho3d/Urho3D.git (the push may not be successful if detached HEAD is not a fast forward of remote master)
  system 'pwd && git config user.name $GIT_NAME && git config user.email $GIT_EMAIL && git remote set-url --push origin https://$GH_TOKEN@github.com/$TRAVIS_REPO_SLUG.git && git add Docs/*API*'
  if system("git commit -q -m 'Travis CI: API documentation update at #{Time.now.utc}.\n[ci #{instruction}]'") && !ENV['PACKAGE_UPLOAD']
    bump_soversion 'Source/Engine/.soversion' or abort 'Failed to bump soversion'
    system "git add Source/Engine/.soversion && git commit --amend -q -m 'Travis CI: API documentation update at #{Time.now.utc}.\n[ci #{instruction}]'" or 'Failed to stage .soversion file'
  end
  system "git push origin HEAD:master -q >/dev/null 2>&1" or abort 'Failed to update API documentation, most likely due to remote master has diverged, the API documentation update will be performed again in the subsequent CI build'
end

# Usage: NOT intended to be used manually (if you insist then try: GIT_NAME=... GIT_EMAIL=... GH_TOKEN=... rake travis_ci_rebase)
desc 'Rebase OSX-CI mirror branch'
task :travis_ci_rebase do
  system 'git config user.name $GIT_NAME && git config user.email $GIT_EMAIL && git remote set-url --push origin https://$GH_TOKEN@github.com/$TRAVIS_REPO_SLUG.git && git fetch origin OSX-CI:OSX-CI && git rebase origin/master OSX-CI && git push -qf -u origin OSX-CI >/dev/null 2>&1' or abort 'Failed to rebase OSX-CI mirror branch'
end

# Usage: NOT intended to be used manually (if you insist then try: rake travis_ci_package_upload)
desc 'Make binary package and upload it to a designated central hosting server'
task :travis_ci_package_upload do
  if ENV['XCODE']
    $configuration = 'Release'
    $testing = 0
  end
  if ENV['ANDROID']
    platform_prefix = 'android-'
  elsif ENV['WINDOWS']
    platform_prefix = 'mingw-'
  elsif ENV['IOS']
    platform_prefix = 'ios-'
  elsif ENV['RPI']
    platform_prefix = 'raspi-'
  else
    platform_prefix = ''
  end
  # Generate the documentation if necessary
  unless ENV['SITE_UPDATE']
    system 'echo Generate documentation'
    if ENV['XCODE']
      xcode_build(ENV['IOS'], "#{platform_prefix}Build/Urho3D.xcodeproj", 'doc', false, '>/dev/null') or abort 'Failed to generate documentation'
    else
      system "cd #{platform_prefix}Build && make -j$NUMJOBS doc >/dev/null" or abort 'Failed to generate documentation'
    end
  end
  # Make the package
  if ENV['IOS']
    unless ENV['URHO3D_64BIT']  # Skip Mach-O universal binary build for the time being as otherwise overall build time exceeds 50 minutes time limit
      # Build Mach-O universal binary consisting of iphoneos (universal ARM archs including 'arm64' if 64-bit is enabled) and iphonesimulator (i386 arch and also x86_64 arch if 64-bit is enabled)
      system 'echo Rebuild Urho3D library as Mach-O universal binary'
      xcode_build(0, "#{platform_prefix}Build/Urho3D.xcodeproj", 'Urho3D_universal', false) or abort 'Failed to build Mach-O universal binary'
    end
    # There is a bug in CMake/CPack that causes the 'package' scheme failed to build for IOS platform, workaround by calling cpack directly
    system "cd #{platform_prefix}Build && cpack -G TGZ 2>/dev/null" or abort 'Failed to make binary package'
  elsif ENV['XCODE']
    xcode_build(ENV['IOS'], "#{platform_prefix}Build/Urho3D.xcodeproj", 'package', false) or abort 'Failed to make binary package'
  else
    if ENV['ANDROID']
      # Build Android package consisting of both armeabi-v7a and armeabi ABIs
      system 'echo Reconfigure and rebuild Urho3D project using armeabi ABI'
      system "SKIP_NATIVE=1 ./cmake_gcc.sh -DANDROID_ABI=armeabi && cd #{platform_prefix}Build && make -j$NUMJOBS" or abort 'Failed to reconfigure and rebuild for armeabi'
      system "cd #{platform_prefix}Build && $ANDROID_SDK/tools/android update project -p . -t 1 && ant debug && bash -c 'mv bin/Urho3D{-debug,}.apk'" or abort 'Failed to make Android package (apk)'
    end
    system "cd #{platform_prefix}Build && make package" or abort 'Failed to make binary package'
  end
  # Determine the upload location
  setup_digital_keys
  if ENV['RELEASE_TAG'].empty?
    upload_dir = "/home/frs/project/#{ENV['TRAVIS_REPO_SLUG']}/Snapshots"
    if ENV['SITE_UPDATE']
      # Download source packages from GitHub
      system 'SNAPSHOP_VER=`git describe $TRAVIS_COMMIT`; export SNAPSHOP_VER && wget -q https://github.com/$TRAVIS_REPO_SLUG/tarball/$TRAVIS_COMMIT -O Urho3D-$SNAPSHOP_VER-Source-snapshot.tar.gz && wget -q https://github.com/$TRAVIS_REPO_SLUG/zipball/$TRAVIS_COMMIT -O Urho3D-$SNAPSHOP_VER-Source-snapshot.zip' or abort 'Failed to get source packages'
      # Only keep the snapshots from the last +/- 50 revisions
      # The package revisions and their creation time may not always be in perfect chronological order due to Travis-CI build latency, so sort the final result one more time in order to get a unique revision removal list
      system "for v in $(sftp urho-travis-ci@frs.sourceforge.net <<EOF |tr ' ' '\n' |grep Urho3D- |cut -d '-' -f1,2 |uniq |tail -n +51 |sort |uniq
cd #{upload_dir}
ls -1t
bye
EOF
); do echo rm #{upload_dir}/${v}*; done |sftp -b - urho-travis-ci@frs.sourceforge.net" or abort 'Failed to housekeep snapshots'
    end
  else
    upload_dir = "/home/frs/project/#{ENV['TRAVIS_REPO_SLUG']}/#{ENV['RELEASE_TAG']}"
    if ENV['SITE_UPDATE']
      # Download source packages from GitHub
      system 'wget -q https://github.com/$TRAVIS_REPO_SLUG/archive/$RELEASE_TAG.tar.gz -O Urho3D-$RELEASE_TAG-Source.tar.gz && wget -q https://github.com/$TRAVIS_REPO_SLUG/archive/$RELEASE_TAG.zip -O Urho3D-$RELEASE_TAG-Source.zip' or abort 'Failed to get source packages'
    end
    # Make sure the release directory exists remotely, do this in all the build jobs as we don't know which one would start uploading first
    system "sftp urho-travis-ci@frs.sourceforge.net <<EOF >/dev/null 2>&1
mkdir #{upload_dir}
bye
EOF" or abort 'Failed to create release directory remotely'
  end
  # Upload the package
  system "scp #{platform_prefix}Build/Urho3D-* urho-travis-ci@frs.sourceforge.net:#{upload_dir}" or abort 'Failed to upload binary package'
  if ENV['SITE_UPDATE']
    # Upload the source package
    system "scp Urho3D-* urho-travis-ci@frs.sourceforge.net:#{upload_dir}" or abort 'Failed to upload source package'
    # Sync readme and license files, just in case they are updated in the repo
    system 'for f in Readme.txt License.txt; do mtime=$(git log --format=%ai -n1 $f); touch -d "$mtime" $f; done' or abort 'Failed to acquire file modified time'
    system 'rsync -e ssh -az Readme.txt License.txt urho-travis-ci@frs.sourceforge.net:/home/frs/project/$TRAVIS_REPO_SLUG' or abort 'Failed to sync readme and license files'
  end
end

def scaffolding(dir, project = 'Scaffolding', target = 'Main')
  system "bash -c \"mkdir -p #{dir}/{Source,Bin} && cp Source/Tools/Urho3DPlayer/Urho3DPlayer.* #{dir}/Source && for f in {.,}*.sh; do ln -sf `pwd`/\\$f #{dir}; done && ln -sf `pwd`/Bin/{Core,}Data #{dir}/Bin\" && cat <<EOF >#{dir}/Source/CMakeLists.txt
# Set project name
project (#{project})

# Set minimum version
cmake_minimum_required (VERSION 2.8.6)

if (COMMAND cmake_policy)
    cmake_policy (SET CMP0003 NEW)
    if (CMAKE_VERSION VERSION_GREATER 2.8.12 OR CMAKE_VERSION VERSION_EQUAL 2.8.12)
        cmake_policy (SET CMP0022 NEW) # INTERFACE_LINK_LIBRARIES defines the link interface
    endif ()
    if (CMAKE_VERSION VERSION_GREATER 3.0.0 OR CMAKE_VERSION VERSION_EQUAL 3.0.0)
        cmake_policy (SET CMP0026 OLD) # Disallow use of the LOCATION target property - therefore we set to OLD as we still need it
        cmake_policy (SET CMP0042 NEW) # MACOSX_RPATH is enabled by default
    endif ()
endif ()

# Set CMake modules search path
set (CMAKE_MODULE_PATH
    \\$ENV{URHO3D_HOME}/Source/CMake/Modules
    \\$ENV{CMAKE_PREFIX_PATH}/share/Urho3D/CMake/Modules
    CACHE PATH \"Path to Urho3D-specific CMake modules\")

# Include Urho3D CMake common module
include (Urho3D-CMake-common)

# Find Urho3D library
find_package (Urho3D REQUIRED)
include_directories (\\${URHO3D_INCLUDE_DIRS})

# Define target name
set (TARGET_NAME #{target})

# Define source files
define_source_files ()

# Setup target with resource copying
setup_main_executable ()

# Setup test cases
add_test (NAME ExternalLibAS COMMAND \\${TARGET_NAME} Data/Scripts/12_PhysicsStressTest.as -w -timeout \\${URHO3D_TEST_TIME_OUT})
add_test (NAME ExternalLibLua COMMAND \\${TARGET_NAME} Data/LuaScripts/12_PhysicsStressTest.lua -w -timeout \\${URHO3D_TEST_TIME_OUT})
EOF" or abort 'Failed to create new project using Urho3D as external library'
end

def makefile_travis_ci
  if ENV['WINDOWS']
    # LuaJIT on MinGW build is not possible on Ubuntu 12.04 LTS as its GCC cross-compiler version is too old. Fallback to use Lua library instead.
    jit = ''
    amalg = ''
    # Lua on MinGW build requires tolua++ tool to be built natively first
    system 'MINGW_PREFIX= ./cmake_gcc.sh -DURHO3D_LIB_TYPE=$URHO3D_LIB_TYPE -DURHO3D_64BIT=$URHO3D_64BIT -DURHO3D_LUA=1 -DURHO3D_TOOLS=0' or abort 'Failed to configure native build for tolua++ target'
    system "cd Build/ThirdParty/toluapp/src/bin && make -j$NUMJOBS" or abort 'Failed to build tolua++ tool'
    ENV['SKIP_NATIVE'] = '1'
    # Temporary workaround due to Travis-CI insufficient memory in MinGW/STATIC build configuration, always use Release configuration.
    $configuration = 'Release'
  else
    jit = 'JIT'
    amalg = '-DURHO3D_LUAJIT_AMALG=1'
  end
  system "./cmake_gcc.sh -DURHO3D_LIB_TYPE=$URHO3D_LIB_TYPE -DURHO3D_64BIT=$URHO3D_64BIT -DURHO3D_LUA#{jit}=1 #{amalg} -DURHO3D_SAMPLES=1 -DURHO3D_TOOLS=1 -DURHO3D_EXTRAS=1 -DURHO3D_TESTING=#{$testing} -DCMAKE_BUILD_TYPE=#{$configuration}" or abort 'Failed to configure Urho3D library build'
  if ENV['ANDROID']
    # LuaJIT on Android build requires tolua++ and buildvm-android tools to be built natively first
    system "cd Build/ThirdParty/toluapp/src/bin && make -j$NUMJOBS" or abort 'Failed to build tolua++ tool'
    system "cd Build/ThirdParty/LuaJIT/generated/buildvm-android && make -j$NUMJOBS" or abort 'Failed to build buildvm-android tool'
    # Reconfigure Android build one more time now that we have the tools built
    ENV['SKIP_NATIVE'] = '1'
    system './cmake_gcc.sh' or abort 'Failed to reconfigure Urho3D library for Android build'
    platform_prefix = 'android-'
  elsif ENV['RPI']
    # LuaJIT on Raspberry Pi build requires tolua++ and buildvm-raspi tools to be built natively first
    system "cd Build/ThirdParty/toluapp/src/bin && make -j$NUMJOBS" or abort 'Failed to build tolua++ tool'
    system "cd Build/ThirdParty/LuaJIT/generated/buildvm-raspi && make -j$NUMJOBS" or abort 'Failed to build buildvm-raspi tool'
    # Reconfigure Raspberry Pi build one more time now that we have the tools built
    ENV['SKIP_NATIVE'] = '1'
    system './cmake_gcc.sh' or abort 'Failed to reconfigure Urho3D library for Raspberry Pi build'
    platform_prefix = 'raspi-'
  elsif ENV['WINDOWS']
    platform_prefix = 'mingw-'
  else
    platform_prefix = ''
  end
  # Temporary workaround due to Travis-CI insufficient memory in MinGW/STATIC build configuration, build samples separately and retry build for 3 times from where it fails
  if ENV['CI'] and ENV['WINDOWS'] and ENV['URHO3D_LIB_TYPE'] == 'STATIC'
    system "cd #{platform_prefix}Build/Tools && make -j$NUMJOBS && cd .. && (make -j$NUMJOBS || make || make)" or abort 'Failed to build or test Urho3D library'
  else
    # Only 64-bit Linux environment with virtual framebuffer X server support and not MinGW build; or OSX build environment are capable to run tests
    if $testing == 1 and (ENV['URHO3D_64BIT'] and ENV['WINDOWS'].to_i != 1 or ENV['OSX'])
      test = '&& make test'
    else
      test = ''
    end
    system "cd #{platform_prefix}Build && make -j$NUMJOBS #{test}" or abort 'Failed to build or test Urho3D library'
  end
  # Create a new project on the fly that uses newly built Urho3D library
  scaffolding "#{platform_prefix}Build/generated/externallib"
  system "URHO3D_HOME=`pwd`; export URHO3D_HOME && cd #{platform_prefix}Build/generated/externallib && echo '\nUsing Urho3D as external library in external project' && ./cmake_gcc.sh -DURHO3D_64BIT=$URHO3D_64BIT -DURHO3D_LUA#{jit}=1 -DURHO3D_TESTING=#{$testing} -DCMAKE_BUILD_TYPE=#{$configuration} && cd #{platform_prefix}Build && make -j$NUMJOBS #{test}" or abort 'Failed to configure/build/test temporary project using Urho3D as external library' 
end

def xcode_travis_ci
  if ENV['IOS']
    # IOS platform does not support LuaJIT
    jit = ''
    amalg = ''
    platform_prefix = 'ios-'
    # Lua on IOS build requires tolua++ tool to be built natively first
    system "./cmake_macosx.sh -DURHO3D_LIB_TYPE=$URHO3D_LIB_TYPE -DURHO3D_64BIT=$URHO3D_64BIT -DURHO3D_LUA=1 -DURHO3D_TOOLS=0" or abort 'Failed to configure native build for tolua++ target'
    xcode_build(0, 'Build/Urho3D.xcodeproj', 'tolua++') or abort 'Failed to build tolua++ tool'
  else
    jit = 'JIT'
    amalg = '-DURHO3D_LUAJIT_AMALG=1'
    platform_prefix = ''
  end
  system "./cmake_macosx.sh -DIOS=$IOS -DURHO3D_LIB_TYPE=$URHO3D_LIB_TYPE -DURHO3D_64BIT=$URHO3D_64BIT -DURHO3D_LUA#{jit}=1 #{amalg} -DURHO3D_SAMPLES=1 -DURHO3D_TOOLS=1 -DURHO3D_EXTRAS=1 -DURHO3D_TESTING=#{$testing}" or abort 'Failed to configure Urho3D library build'
  xcode_build(ENV['IOS'], "#{platform_prefix}Build/Urho3D.xcodeproj") or abort 'Failed to build or test Urho3D library'
  # Create a new project on the fly that uses newly built Urho3D library
  scaffolding "#{platform_prefix}Build/generated/externallib"
  system "URHO3D_HOME=`pwd`; export URHO3D_HOME && cd #{platform_prefix}Build/generated/externallib && echo '\nUsing Urho3D as external library in external project' && ./cmake_macosx.sh -DIOS=$IOS -DURHO3D_64BIT=$URHO3D_64BIT -DURHO3D_LUA#{jit}=1 -DURHO3D_TESTING=#{$testing}" or abort 'Failed to configure temporary project using Urho3D as external library'
  xcode_build(ENV['IOS'], "#{platform_prefix}Build/generated/externallib/#{platform_prefix}Build/Scaffolding.xcodeproj") or abort 'Failed to configure/build/test temporary project using Urho3D as external library'
end

def xcode_build(ios, project, scheme = 'ALL_BUILD', autosave = true, extras = '')
  if autosave
    # Save auto-created schemes from Xcode project file
    system "ruby -i -pe 'gsub(/refType = 0; /, %q{})' #{project}/project.pbxproj" or 'Failed to remove legacy refType attributes from the generated Xcode project file'
    system "ruby -i -e 'puts ARGF.read.gsub(/buildSettings = \\{\n\s*?\\};\n\s*?buildStyles = \(.*?\);/m, %q{})' #{project}/project.pbxproj" or 'Failed to remove unsupported PBXProject attributes from the generated Xcode project file'
    xcproj = Xcodeproj::Project.open(project)
    xcproj.recreate_user_schemes
    xcproj.save     # There is a bug in this gem, it does not appear to exit with proper exit status (assume always success for now)
  end
  sdk = ios.to_i == 1 ? '-sdk iphonesimulator' : ''
  # Use xctool when building as its output is nicer
  system "xctool -project #{project} -scheme #{scheme} -configuration #{$configuration} #{sdk} #{extras}" or return 1
  if $testing == 1 and ios.to_i != 1 and scheme == 'ALL_BUILD'     # Disable testing for IOS as we don't have unit tests for IOS platform yet
    # Use xcodebuild when testing as its output is instantaneous (ensure Travis-CI does not kill the process during testing)
    system "xcodebuild -project #{project} -scheme RUN_TESTS -configuration #{$configuration} #{sdk} #{extras}" or return 1
  end
  return 0
end

def append_new_release release, filename
  begin
    urho3d_hash = JSON.parse File.read filename
    unless urho3d_hash['releases'].last == release
      urho3d_hash['releases'] << release
    end
    File.open filename, 'w' do |file|
      file.puts urho3d_hash.to_json
    end
    return 0
  rescue
    nil
  end
end

def bump_soversion filename
  begin
    version = File.read(filename).split '.'
    bump_version version, 2
    File.open filename, 'w' do |file|
      file.puts version.join '.'
    end
    return 0
  rescue
    nil
  end
end

def bump_version version, index
  if index > 0 && version[index].to_i == 255
    version[index] = 0
    bump_version version, index - 1
  else
    version[index] = version[index].to_i + 1
  end
end

def setup_digital_keys
  system 'mkdir -p ~/.ssh && chmod 700 ~/.ssh' or abort 'Failed to create ~/.ssh directory'
  system "cat <<EOF >>~/.ssh/known_hosts
frs.sourceforge.net,216.34.181.57 ssh-rsa AAAAB3NzaC1yc2EAAAABIwAAAQEA2uifHZbNexw6cXbyg1JnzDitL5VhYs0E65Hk/tLAPmcmm5GuiGeUoI/B0eUSNFsbqzwgwrttjnzKMKiGLN5CWVmlN1IXGGAfLYsQwK6wAu7kYFzkqP4jcwc5Jr9UPRpJdYIK733tSEmzab4qc5Oq8izKQKIaxXNe7FgmL15HjSpatFt9w/ot/CHS78FUAr3j3RwekHCm/jhPeqhlMAgC+jUgNJbFt3DlhDaRMa0NYamVzmX8D47rtmBbEDU3ld6AezWBPUR5Lh7ODOwlfVI58NAf/aYNlmvl2TZiauBCTa7OPYSyXJnIPbQXg6YQlDknNCr0K769EjeIlAfY87Z4tw==
EOF" or abort 'Failed to append frs.sourceforge.net server public key to known_hosts'
  # Workaround travis encryption key size limitation. Rather than using the solution in their FAQ (using AES to encrypt/decrypt the file and check in the encrypted file into repo), our solution is more pragmatic. The private key below is incomplete. Only the missing portion is encrypted. Much less secure than the original 2048-bit RSA has to offer but good enough for our case. 
  system "cat <<EOF >~/.ssh/id_rsa
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEAnZGzFEypdXKY3KDT0Q3NLY4Bv74yKgJ4LIgbXothx8w4CfM0
VeWBL/AE2iRISEWGB07LruM9y+U/wt58WlCVu001GuJuvXwWenlljsvH8qQlErYi
oXlCwAeVVeanILGL8CPS7QlyzOwwnVF6NdcmfDJjTthBVFbvHrWGo5if86zcZyMR
2BB5QVEr5fU0yOPFp0+2p7J3cA6HQSKwjUiDtJ+lM62UQp7InCCT3qeh5KYHQcYb
KVJTyj5iycVuBujHDwNAivLq82ojG7LcKjP+Ia8fblardCOQyFk6pSDM79NJJ2Dg
3ZbYIJeUmqSqFhRW/13Bro7Z1aNGrdh/XZkkHwIDAQABAoIBACHcBFJxYtzVIloO
yVWcFKIcaO3OLjNu0monWVJIu1tW3BfvRijLJ6aoejJyJ4I4RmPdn9FWDZp6CeiT
LL+vn21fWvELBWb8ekwZOCSmT7IpaboKn4h5aUmgl4udA/73iC2zVQkQxbWZb5zu
vEdDk4aOwV5ZBDjecYX01hjjnEOdZHGJlF/H/Xs0hYX6WDG3/r9QCJJ0nfd1/Fk2
zdbZRtAbyRz6ZHiYKnFQ441qRRaEbzunkvTBEwu9iqzlE0s/g49LJL0mKEp7rt/J
4iS3LZTQbJNx5J0ti8ZJKHhvoWb5RJxNimwKvVHC0XBZKTiLMrhnADmcpjLz53F8
$SF_KEY
sx27yCaeBeKXV0tFOeZmgK664VM9EgesjIX4sVOJ5mA3xBJBOtz9n66LjoIlIM58
dvsAnJt7MUBdclL/RBHEjbUxgGBDcazfWSuJe0sGczhnXMN94ox4MSECgYEAx5cv
cs/2KurjtWPanDGSz71LyGNdL/xQrAud0gi49H0tyYr0XmzNoe2CbZ/T5xGSZB92
PBcz4rnHQ/oujo/qwjNpDD0xVLEU70Uy/XiY5/v2111TFC4clfE/syZPywKAztt3
y2l5z+QdsNigRPDhKw+7CFYaAnYBEISxR6nabT8CgYEAqHrM8fdn2wsCNE6XvkZQ
O7ZANHNIKVnaRqW/8HW7EFAWQrlQTgzFbtR4uNBIqAtPsvwSx8Pk652+OR1VKfSv
ya3dtqY3rY/ErXWyX0nfPQEbYj/oh8LbS6zPw75yIorP3ACIwMw3GRNWIvkdAGTn
BMUgpWHUDLWWpWRrSzNi90ECgYEAkxxzQ6vW5OFGv17/NdswO+BpqCTc/c5646SY
ScRWFxbhFclOvv5xPqYiWYzRkmIYRaYO7tGnU7jdD9SqVjfrsAJWrke4QZVYOdgG
cl9eTLchxLGr15b5SOeNrQ1TCO4qZM3M6Wgv+bRI0h2JW+c0ABpTIBzehOvXcwZq
6MhgD98CgYEAtOPqc4aoIRUy+1oijpWs+wU7vAc8fe4sBHv5fsv7naHuPqZgyQYY
32a54xZxlsBw8T5P4BDy40OR7fu+6miUfL+WxUdII4fD3grlIPw6bpNE0bCDykv5
RLq28S11hDrKf/ZetXNuIprfTlhl6ISBy+oWQibhXmFZSxEiXNV6hCQ=
-----END RSA PRIVATE KEY-----
EOF" or abort 'Failed to create user private key to id_rsa'
  system 'chmod 600 ~/.ssh/id_rsa' or abort 'Failed to change id_rsa file permission'
end

# vi: set ts=2 sw=2 expandtab:
