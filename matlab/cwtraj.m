% ���ݳ�ʼ״̬����CW�켣
function traj = cwtraj(x0,w,T,h)
if ~isvector(x0)
    error('�����ʼ״̬x0����Ϊʸ��');
end
[m n] = size(x0);
dim = max(m,n);
if ~(dim==4 || dim==6)
    error('�����ʼ״̬����Ϊ4ά��6άʸ��');
end
t = [0:h:T T];
len = length(t);
traj = nan(dim,len);
if m==1
    x0 = x0'; %ת����ʸ��
end
for i=1:len
    if dim==4
        phi = cwstm4(w,t(i));
    else
        phi = cwstm6(w,t(i));
    end
    traj(:,i) = phi*x0;
end
traj = traj';
