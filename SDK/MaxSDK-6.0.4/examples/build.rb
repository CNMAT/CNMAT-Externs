#!/usr/bin/env ruby -wKU
# encoding: utf-8

# Build all Max SDK examples on the Mac
# Run from the terminal by cd'ing into the examples directory, and then running:
#   ./build.rb

sdk_examples_dir = "."
Dir.chdir sdk_examples_dir
sdk_examples_dir = Dir.pwd


# method to recursively build the projects

def build_projects_for_dir(path)
  puts
  puts "Building Projects for Directory: #{path}"
  
  Dir.foreach path do |filename|
    if filename.match(/.*\.xcodeproj/)
      puts "  Building #{filename}"
      result = `cd #{path}; xcodebuild -project #{filename} 2>&1`
      if result.match(/\*\* BUILD SUCCEEDED \*\*/)
        puts "    (success)"
      else
        puts "    (FAIL) ************************************"
      end
    elsif File.directory?("#{path}/#{filename}") && filename != "." && filename != ".." && filename != "build"
      build_projects_for_dir("#{path}/#{filename}")
    end
  end
  
  puts
end


build_projects_for_dir(sdk_examples_dir)

