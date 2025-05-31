input_path="bun_zipper_res4.ply"
output_path="bun_zipper_res4.obj"

File.open(input_path, "r") { |file|
  vcount=icount=nil
  while(line=file.gets)
    #自分自身をわける
    line.strip!
    case line
    when /^element vertex (\d+)/
      vcount = $1.to_i
    when /^element face (\d+)/
      icount = $1.to_i
    when /^end_header/
      break
    end
  end


    vertices=Array.new(vcount) {
    parts=file.gets.split
    parts[0,3].map{ |i| i.to_f }
    }
    #面ごとにインデックスが入った２次元配列
    indices=Array.new(icount) {
    parts=file.gets.split.map{ |i| i.to_i }
    #１つ目の要素の数だけ２つ目の要素から数えて取り出す
    parts[1,parts[0]]
    }

    File.open(output_path, "w") { |out_file|
      vertices.each { |v|
        out_file.puts "v #{v[0]} #{v[1]} #{v[2]}"}
      indices.each { |face|
        out_file.puts "f #{face.map { |i| i + 1 }.join(' ')}"
        }

  }
}
