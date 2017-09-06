% ��ȡhdf5�ļ�,���ļ��е�Dataset������������������ȡ����caller'�Ŀռ���
% readhdf5(file)
% TODO:  ��һ��viewhdf5����

function DatasetNames = readhdf5(file)
if nargin == 0 %self test
    file = 'D:\Project\hdf5test\pt.h5';
end
info = h5info(file);
np = length(info.Datasets);
DatasetNames = cell(np,1);
for k=1:np
    varname = info.Datasets(k).Name;
    DatasetNames{k} = varname;
    exp = [varname,' = h5read(''',file,''',''/',varname,''');'];
    evalin('caller',exp);
    evalin('caller',[varname ' = squeeze(' varname ');']);
end
% clear file info np exp k varname