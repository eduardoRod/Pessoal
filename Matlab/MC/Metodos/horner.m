function [y,b]=horner(a,z)
n=length(a)-1;
b=zeros(n+1,1);
b(1)=a(1);
for j=2:n+1
    b(j)=a(j)+b(j-1)*z;
end
y=b(n+1);
b=b(1:end-1);
return