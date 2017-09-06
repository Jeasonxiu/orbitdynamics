% �����������ٶȶ�λ��ʸ�����ſ˱Ⱦ���
%   �������ٶȽ�������������
%   j = jac_dadr(r)
%   ����rΪ��άλ��ʸ�����õ�3*3���ſ˱Ⱦ���
%   ����rΪ��άλ���ٶ�ʸ�����õ�6*6���ſ˱Ⱦ��󣬾����ֵ���ٶ��޹�
%      (   Miu  [ x ] )
%    d (- ----- [ y ] )
%      (   r^3  [ z ] )
%J = ------------------
%          [ x ]
%        d [ y ]
%          [ z ]
function j = jac_dadr_2body(r)
r = r(:);
m = size(r,1);
if m == 3
    dim = 3;
elseif m == 6;
    dim = 6;
    r = r(1:3);
else
    error('����ʸ��ά�����ԣ�ӦΪ3άλ�û���6άλ���ٶ�');
end
radius = sqrt(r(1)^2+r(2)^2+r(3)^2);
j = GEarth/radius^3*(3*r*r'/radius^2 - eye(3));
if dim == 6
    j = [zeros(3,3) eye(3);j  zeros(3,3)];
end