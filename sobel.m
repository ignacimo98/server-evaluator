pkg load image

args = argv();
A = imread(args{1});

[m,n,ch] = size(A);

A = double(A);

# Creamos matriz B
B1 = [-1,0,1;-2,0,2;-1,0,1]; # Detección de bordes verticales
B2 = B1'; # Detección de bordes horizontales

D1 = zeros(m,n,ch);
D2 = zeros(m,n,ch);
D1(:,:,1) = (conv2(A(:,:,1),B1)(2:m+1,2:n+1));
D2(:,:,1) = (conv2(A(:,:,1),B2)(2:m+1,2:n+1));

if (ch>1)
  D1(:,:,2) = (conv2(A(:,:,2),B1)(2:m+1,2:n+1));
  D2(:,:,2) = (conv2(A(:,:,2),B2)(2:m+1,2:n+1));
  D1(:,:,3) = (conv2(A(:,:,3),B1)(2:m+1,2:n+1));
  D2(:,:,3) = (conv2(A(:,:,3),B2)(2:m+1,2:n+1));
endif

E = sqrt(D1.^2+D2.^2);

imwrite(uint8(E), args{1});