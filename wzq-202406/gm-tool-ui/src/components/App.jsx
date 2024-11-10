import React from 'react';
import {Outlet, useLocation} from 'react-router-dom';
import {Avatar, Button, Layout, Nav} from '@douyinfe/semi-ui';
import {
    IconFaceuLogo,
    IconHome,
    IconMail,
    IconMicrophone,
    IconUser,
    IconWrench,
    IconGift,
} from '@douyinfe/semi-icons';

import {useServer} from '../utils/useServer';
import Login from './Login';

export default function Root() {
    const location = useLocation();
    const {user, fetchAdminInfo} = useServer();
    if (user == null) {
        return <Login/>;
    }

    const items = [
        {itemKey: 'Home', text: '首页', icon: <IconHome size="large"/>, link: '/'},
        {
            itemKey: 'User', text: '用户管理', icon: <IconUser size="large"/>,
            items: [
                {
                    itemKey: 'User/List',
                    text: '用户列表',
                    link: '/user/list',
                },
            ],
        },
        {
            itemKey: 'Setting', text: '设置', icon: <IconWrench size="large"/>,
            items: [
                {
                    itemKey: 'Setting/AuthCode',
                    text: '手机号登录验证码设置',
                    link: '/setting/authCode',
                },
            ]
        }
    ];

    const findKey = itemList => {
        for (let i = 0; i < itemList.length; i++) {
            const item = itemList[i];
            if (item.matchLink && item.matchLink.test(location.pathname)) {
                return item.itemKey;
            } else if (item.link === location.pathname) {
                return item.itemKey;
            } else if (item.items) {
                const res = findKey(item.items);
                if (res) {
                    return res;
                }
            }
        }
        return '';
    };

    const curKey = findKey(items);

    const {Header, Sider, Content} = Layout;
    return (
        <Layout style={{border: '1px solid var(--semi-color-border)'}}>
            <Sider style={{backgroundColor: 'var(--semi-color-bg-1)'}}>
                <Nav
                    defaultSelectedKeys={['Home']}
                    style={{maxWidth: 220, height: '100%'}}
                    items={items}
                    header={{
                        text: '李梓涛五子棋 GM后台',
                    }}
                    footer={{
                        collapseButton: true,
                    }}
                    selectedKeys={[curKey]}
                />
            </Sider>
            <Layout>
                <Header style={{backgroundColor: 'var(--semi-color-bg-1)'}}>
                    <Nav
                        mode="horizontal"
                    />
                </Header>
                <Content
                    style={{
                        padding: '24px',
                        backgroundColor: 'var(--semi-color-bg-0)',
                    }}
                >
                    <div
                        style={{
                            borderRadius: '10px',
                            border: '1px solid var(--semi-color-border)',
                            minHeight: '700px',
                            padding: '32px',
                        }}
                    >
                        <Outlet/>
                    </div>
                </Content>
            </Layout>
        </Layout>
    );
};
