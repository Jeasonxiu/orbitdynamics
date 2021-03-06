% CW方程状态转移矩阵
function phi = cwstm6(w,t)
wt = w*t;
s = sin(wt);
c = cos(wt);
phi = [1   0   6*(wt-s)   4*s/w-3*t   0    2/w*(1-c); ...
       0   c    0             0       s/w      0;...
       0   0   4-3*c      -2/w*(1-c)   0      s/w;...
       0   0   6*w*(1-c)   4*c-3       0      2*s;...
       0  -s*w   0           0         c       0;...
       0   0   3*w*s        -2*s        0      c];