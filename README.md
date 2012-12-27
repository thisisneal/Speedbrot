Speedbrot
=========

Fast mandelbrot rendering in C

Batch crop all png's in this folder, size 1280x720, vertical offset 280 :
mogrify -crop 1280x720+0+280 *.png

Create a video from all the frame###.png images in this folder, using h264 :
 - 0 is framerate
# NOTE THAT ORDER MATTERS A LOT
ffmpeg -i frame%03d.png -preset slow -r 25 -pix_fmt yuv420p -vcodec libx264 vid11.mp4