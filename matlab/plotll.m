% ����γ��ͼ
% �Ծ��ȷ����������нضϣ�����������
% ���Ա������켣����������
function plotll(lon,lat)
if nargin == 0
    warning('self test');
    lon = 0:1:1500;
    lat = sind(5.3*lon+0.1)*40+lon/3000;
    lon = mod(lon,360);
end
idx = find(abs(diff(lon))>180);
plot(lon(1:idx(1)),lat(1:idx(1)));hold on;
for i=1:length(idx)-1
    plot(lon(idx(i)+1:idx(i+1)),lat(idx(i)+1:idx(i+1)));
end
plot(lon(idx(i+1)+1:end),lat(idx(i+1)+1:end));
figure,plot(lon,lat);