% ���Ƕ����Ӽ�360���0������ֵ
function y = limit360(x)
flag = x>180;
x = x - 360*flag;
flag = x<-180;
y = x + 360*flag;