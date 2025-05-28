input_path="bun_zipper.ply"
output_path="bun_zipper.obj"

File.open(input_path, "r") do |file|{
  vcount=icount=nil
  while(line=file.gets){
    line.strip!
    case line
    when /^element vertex (\d+)/
      vcount = $1.to_i
    when /^element face (\d+)/
      icount = $1.to_i
    when /^end_header/
      break
    end
  }
}