import React from 'react';
import {Empty} from '@douyinfe/semi-ui';
import {IllustrationConstruction, IllustrationConstructionDark} from '@douyinfe/semi-illustrations';

export default function ErrorPage() {
    return (
        <Empty
            image={<IllustrationConstruction style={{width: 150, height: 150}}/>}
            darkModeImage={<IllustrationConstructionDark style={{width: 150, height: 150}}/>}
            title={'页面404'}
            description='进入到了异次元'
        />
    );
}
