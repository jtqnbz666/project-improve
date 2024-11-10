import React, {useState, useContext, createContext} from 'react';

const serverContext = createContext();

export function ProvideServer({children}) {
    const server = useProvideServer();
    return <serverContext.Provider value={server}>{children}</serverContext.Provider>;
}

export const useServer = () => {
    // 所有ProvideServer的子组件可以通过这个方法调用server对象，看最外层的index.jsx的用法
    return useContext(serverContext);
};

function useAuth() {
    const id = localStorage.getItem('id');
    const name = localStorage.getItem('name');
    const avatar = localStorage.getItem('avatar');
    const token = localStorage.getItem('token');
    const envName = localStorage.getItem('envName');

    const hasValue = o => true//o !== null && o !== undefined && o !== '';
    const initUser = hasValue(id) && hasValue(name) && hasValue(avatar) && hasValue(token) && hasValue(envName) ?
        {
            id,
            name,
            avatar,
            token,
            env: {
                name: envName
            },
        } : null;

    const [user, setUser] = useState(initUser);

    const save = (token, user, env) => {
        localStorage.setItem('id', user.id);
        localStorage.setItem('name', user.name);
        localStorage.setItem('avatar', user.avatar);
        localStorage.setItem('token', token);
        localStorage.setItem('envName', env.name);
        setUser({
            id: user.id,
            name: user.name,
            avatar: user.avatar,
            token: token,
            env: {
                name: env.name
            },
        });
    };

    const clean = () => {
        localStorage.removeItem('token');
        localStorage.removeItem('id');
        localStorage.removeItem('name');
        localStorage.removeItem('avatar');
        localStorage.removeItem('envName');
        setUser(null);
    };

    return {
        user,
        save,
        clean,
    };
}

function useProvideServer() {
    const auth = useAuth();
    const [fetchError, setFetchError] = useState('');

    const hostPrefix = 'http://127.0.0.1:21328/api';

    const generateAuth = () => {
        const id = auth.user?.id;
        const token = auth.user?.token;
        return 'Basic ' + btoa(id + ':' + token);
    };

    const request = async (url, data) => {
        //window.alert(`发送请求 ${url}`);
        const resp = await fetch(`${hostPrefix}${url}`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': generateAuth(),
            },
            body: JSON.stringify(data)
        });
        if (resp.status === 401) {
            auth.clean();
            setFetchError(res.reason);
            return res;
        }
        const res = await resp.json();
        if (!res) {
            setFetchError('请求失败，没有返回值');
            return {
                success: false,
            };
        }
        if (res.success) {
            return res;
        }
        setFetchError(res.err.toString());
        return res;
    };

    const fetchUserList = async (order, limit, offset, condition) => {
        return request('/user/list', {
            order,
            limit,
            offset,
            condition
        });
    };

    // 设置验证码
    const setAuthCode = async msg => {
        return request('/setting/authCode', msg);
    };


    return {
        user: auth.user,
        request,
        fetchError,
        fetchUserList,
        setAuthCode,
    };
}
