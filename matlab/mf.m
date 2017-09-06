%  ƽ�����Mת��Ϊ������f
%  fM(e,f)
function f = mf(e,M)
EE = keplerfunc(e,M);
cosf = (cos(EE)-e) / (1-e*cos(EE));
sinf = sqrt(1-e^2)*sin(EE)/(1-e*cos(EE));
f = atan2(sinf,cosf);
if f<0
    f=f+pi*2;
end
