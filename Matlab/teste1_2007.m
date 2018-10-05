%para se utilizar este programa, terá de se ter um vector
%previamente definido. 
%atribuindo valores a 'x' e 'y'

x=input('Introduza um vector "x": ');
y=input('Introduza uma função: ');

a=0;
b=0;
c=0;
d=0;

%==================================

if y>=0 & y<=1   %[0, 1]
    a=a+1;
end

%==================================

if y>1 & y<=2    %]1, 2]
    b=b+1;
end

%==================================

if y>2 & y<=3    %]2, 3]
    c=c+1;
end

%==================================

if y>3 & y<4   %]3, 4]
    d=d+1;
end

%==================================

disp(['\nA função introduzida tem ' size(a) ' valores compreendidos em [0,1]']);
disp(['\nA função introduzida tem ' size(b) ' valores compreendidos em ]1,2]']);
disp(['\nA função introduzida tem ' size(c) ' valores compreendidos em ]2,3]']);
disp(['\nA função introduzida tem ' size(d) ' valores compreendidos em ]3,4]']);