# siftwin32 <pgm/cd1.pgm >key/cd1.key
# siftwin32 <pgm/scene2.pgm >key/scene2.key 


make %1.pgm
make %1.key
make %2.pgm
make %2.key


match -im1 pgm\%1.pgm -k1 key\%1.key -im2 pgm\%2.pgm -k2 key\%2.key  >out.pgm
