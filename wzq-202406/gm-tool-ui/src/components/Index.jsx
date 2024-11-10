import React from 'react';
import {Empty} from '@douyinfe/semi-ui';
import {IllustrationNoContent, IllustrationNoContentDark} from '@douyinfe/semi-illustrations';

import {useServer} from '../utils/useServer';

export default function Index() {
    const {user} = useServer();
    return (
        <Empty
            image={<IllustrationNoContent style={{width: 150, height: 150}}/>}
            darkModeImage={<IllustrationNoContentDark style={{width: 150, height: 150}}/>}
            title={'GM 主页'}
            description={`你好, ${user.name}`}
        />
    );
}
