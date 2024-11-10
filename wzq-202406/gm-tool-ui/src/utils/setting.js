import {Modal} from '@douyinfe/semi-ui';

export const submit = async (server, formApi, buildKv, validate) => {
    if (validate) {
        const res = await validate(formApi.getValues());
        let hasError = false;
        Object.keys(res).forEach(k => {
            formApi.setError(k, res[k]);
            if (res[k]) {
                hasError = true;
            }
        });
        if (hasError) {
            return;
        }
    }

    Modal.confirm({
        title: '确认保存吗？',
        onOk: async () => {
            const res = await server.saveConf({
                map: buildKv(),
            });
            if (res.success) {
                Modal.success(
                    {
                        title: '保存成功',
                        hasCancel: false,
                    }
                );
            } else {
                Modal.error({
                    title: '保存失败',
                    content: res.err,
                    hasCancel: false,
                });
            }
        },
    });
};

export const simpleBuildKv = (fieldMap, formApi) => {
    return () => {
        const res = {};
        for (let field in fieldMap) {
            res[fieldMap[field]] = getRawData(field, formApi);
        }
        return res;
    };
};

export const getRawData = (field, formApi) => {
    const val = formApi.getValue(field);
    let resV = '';
    if (val !== 'undefined' && val !== undefined && val !== null) {
        resV = val + '';
    }
    return resV;
};

export const simpleInitVal = (fieldMap, confMap, formApi) => {
    for (let key in fieldMap) {
        formApi.setValue(key, confMap[fieldMap[key]]);
    }
};
