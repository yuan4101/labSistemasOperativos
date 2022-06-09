for varVideo in *.mkv
do
    ffmpeg -i $varVideo -codec copy ${varVideo%.*}.mp4
done