pkg load image

args = argv();
A = imread(args{1});

[m,n] = size(A);

A = double(A);

# Creamos matriz B
B1 = [-1,0,1;-2,0,2;-1,0,1]; # Detección de bordes verticales
B2 = B1'; # Detección de bordes horizontales
D1 = (conv2(A,B1)(2:m+1,2:n+1));
D2 = (conv2(A,B2)(2:m+1,2:n+1));
E = sqrt(D1.^2+D2.^2);

imwrite(uint8(E), args{2});