% ������Ŀ����ո���(a,e,i,Omega,w,M)����������f�ı仯�ʺͽǼ��ٶ�
function [df,ddf] = dotf(k)
e = k(2);
a = k(1);
p = a*(1-e^2);
f = ma2ta(e,k(6));
r = p/(1+e*cos(f));
df = sqrt(GEarth*p)/r^2;
rdot = sqrt(GEarth/a/(1-e^2))*e*sin(f);
if nargout > 1
    ddf = -2*rdot.*df./r;
end