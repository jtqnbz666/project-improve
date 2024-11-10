import React, {useEffect, useState} from 'react';
import {useNavigate} from 'react-router-dom';
import {Typography, Divider, Table, Button, Modal, Form, Card, Row, Col, useFormApi} from '@douyinfe/semi-ui';
import {useServer} from '../../utils/useServer';

const {Column} = Table;
const perPage = 10;

let server;
let navigate;
let fetchPage;

function useFetchPage() {
    const [dataList, setDataList] = useState([]);
    const [total, setTotal] = useState(0);
    const [loading, setLoading] = useState(false);
    const [curPage, setCurPage] = useState(1);
    const fetch = async (order, page) => {
        if (page < 1) {
            page = 1;
        }
        setCurPage(page);
        setLoading(true);
        const url = new URLSearchParams(window.document.location.search);
        const res = await server.fetchUserList(order, perPage, (page - 1) * perPage, {
            DisplayID: url.get('DisplayID'),
            UserName: url.get('UserName'),
            UserID: url.get('UserID'),
            PhoneNumber: url.get('PhoneNumber'),
        });
        if (res.success) {
            setDataList(res.list ? res.list : []);
            setTotal(res.total);
        } else {
            Modal.error({
                title: '请求用户列表失败',
                content: res.err,
                hasCancel: false,
            });
        }
        setLoading(false);
    };
    return {
        dataList,
        total,
        loading,
        curPage,
        fetch
    };
}

export default function UserList() {
    server = useServer();
    navigate = useNavigate();
    fetchPage = useFetchPage();

    const {Title} = Typography;
    return (
        <>
            <Title heading={5}>用户列表</Title>
            <Divider style={{marginTop: 10, marginBottom: 10}}/>
            <Card>
                <Form>
                    <SearchArea/>
                </Form>
            </Card>
            <TableArea/>
        </>
    );
}

function TableArea() {
    useEffect(() => {
        fetchPage.fetch('id desc', 1).then();
    }, []);

    const renderName = name => {
        if (name === undefined) {
            return <span style={{color: '#F00'}}>(未设置)</span>;
        }
        return <span>{name}</span>;
    };

    const renderTime = time => {
        const str = new Date(time).toLocaleString();
        return <span>{str}</span>;
    };

    const renderOp = id => {
        return <Button theme={'solid'} type={'secondary'} onClick={() => {
            navigate(`/user/info/${id}`);
        }}>查看详情</Button>;
    };

    const handlePageChange = page => {
        fetchPage.fetch('id desc', page);
    };

    const renderNameOrPhone = (text, record) => {
        const { Account, PhoneNumber } = record;
        if (!Account && !PhoneNumber) {
            return <span style={{ color: '#F00' }}>(未设置)</span>;
        }
        return <span>{Account || PhoneNumber}</span>;
    };

    return (
        <Table dataSource={fetchPage.dataList} loading={fetchPage.loading}
               pagination={{
                   currentPage: fetchPage.curPage,
                   pageSize: perPage,
                   total: fetchPage.total,
                   onPageChange: handlePageChange,
               }}
        >
            <Column title="内部ID" dataIndex="ID" key="id"/>
            <Column title="外显ID" dataIndex="DisplayID" key="DisplayID"/>
            <Column title="名字" dataIndex="Name" key="name" render={renderName}/>
            <Column title="手机号/账号" dataIndex="PhoneNumber" key="phoneNumber" render={renderNameOrPhone}/>
            <Column title="最后登录时间" dataIndex="LastLoginTime" key="loginTime" render={renderTime}/>
            <Column title="" dataIndex="ID" key="operate" render={renderOp}/>
        </Table>
    );
}

function SearchArea() {
    const formApi = useFormApi();
    useEffect(() => {
        const url = new URLSearchParams(window.document.location.search);
        formApi.setValue('DisplayID', url.get('DisplayID'));
        formApi.setValue('UserName', url.get('UserName'));
        formApi.setValue('UserID', url.get('UserID'));
        formApi.setValue('PhoneNumber', url.get('PhoneNumber'));
    });
    const getValue = key => {
        let res = formApi.getValue(key);
        if (res === undefined) return '';
        if (res === null) return '';
        return res;
    };
    return (
        <Form.Section text={'搜索(四选一即可)'}>
            <Row>
                <Col span={6}>
                    <Form.Input field="PhoneNumber" label="手机号" style={{width: 200}}/>
                </Col>
                <Col span={6}>
                    <Form.Input field="UserName" label="名字(模糊搜索)" style={{width: 200}}/>
                </Col>
                <Col span={6}>
                    <Form.Input field="DisplayID" label="外显ID" style={{width: 200}}/>
                </Col>
                <Col span={6}>
                    <Form.Input field="UserID" label="内部ID" style={{width: 200}}/>
                </Col>
                <Col span={12}>
                    <Button type="primary" onClick={() => {
                        window.location.search = `?DisplayID=${getValue('DisplayID')}&UserName=${getValue('UserName')}&UserID=${getValue('UserID')}&PhoneNumber=${getValue('PhoneNumber')}`;
                    }}>查询</Button>
                </Col>
            </Row>
            <Row>
            </Row>
        </Form.Section>
    );
}
