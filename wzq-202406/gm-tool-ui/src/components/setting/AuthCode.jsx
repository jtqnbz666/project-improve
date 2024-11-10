import React, {useEffect, useState} from 'react';
import {Input, Form, Col, Row, Button, useFormApi, Modal} from '@douyinfe/semi-ui';
import {useServer} from '../../utils/useServer';

let server;

const SetForm = props => {
    const formApi = useFormApi();
    const [phone, setPhone] = useState('');
    const [sending, setSending] = useState(false);

    const submit = async () => {
        try {
            await formApi.validate();
        } catch (e) {
            if (Object.keys(e).length !== 0) {
                console.log('校验报错', e);
                return;
            }
        }finally{
            Modal.confirm({
                title: '手机号登录验证码设置',
                onOk: confirmSubmit,
            });
        }
    };
    const confirmSubmit = async () => {
        if (sending) {
            return false;
        }
        setSending(true);
        const res = await server.setAuthCode({
            phone: phone,
        });

        if (res.success) {
            Modal.success({
                title: '设置成功',
                hasCancel: false,
                onOk: () => setSending(false),
            });
        } else {
            Modal.error({
                title: '设置失败',
                content: res.err,
                hasCancel: false,
                onOk: () => setSending(false),
            });
        }
    }

    return (
        <div>
            <Form.Section text={'设置登录验证码为123456，有效时长1小时'} style={{textAlign: 'center', color: 'blue'}}>
                <Row style={{ justifyContent: 'center', display: 'flex', marginBottom: '20px'}}>
                    <Col>
                        <Input placeholder="手机号" onChange={(e) => setPhone(e)} style={{width:'100%'}} />
                    </Col>
                </Row>
                <Row style={{ justifyContent: 'center', display: 'flex' }}>
                    <Col>
                        <Button type="primary" htmlType="submit" loading={sending} onClick={submit}>
                            确认设置
                        </Button>
                    </Col>
                </Row>
            </Form.Section>
        </div>
    );
};

export default function SettingAuthCode() {
    server = useServer();
    return (
        <Form
            style={{padding: 10, width: '100%'}}
            labelPosition={'left'}
            labelWidth={'180px'}
            labelAlign={'left'}
            wrapperCol={{span: 12}}
            labelCol={{span: 3}}
        >
            <SetForm />
        </Form>
    );
};