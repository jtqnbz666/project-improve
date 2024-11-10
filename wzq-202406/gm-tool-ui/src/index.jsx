import React from 'react';
import ReactDOM from 'react-dom/client';
import {
    createBrowserRouter,
    RouterProvider,
} from 'react-router-dom';
import 'reset-css';

import UserList from './components/user/UserList';
import ErrorPage from "./components/ErrorPage";
import Index from './components/Index';
import App from './components/App';
import {ProvideServer} from './utils/useServer';
import SettingAuthCode from "./components/setting/AuthCode";
const router = createBrowserRouter([
    {
        path: '/',
        element: <App/>,
        errorElement: <ErrorPage/>,
        children: [
            {
                path: '',
                element: <Index/>
            },
            {
                path: '/user/list',
                element: <UserList/>
            },
            {
                path: '/setting/authCode',
                element: <SettingAuthCode/>
            },
        ],
    },
]);

ReactDOM.createRoot(document.getElementById('root')).render(
    //<React.StrictMode>
    <ProvideServer>
        <RouterProvider router={router}/>
    </ProvideServer>
    //</React.StrictMode>
);
