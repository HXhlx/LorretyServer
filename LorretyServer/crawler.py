from requests import get
from mysql.connector import connect


def test():
    print("Python脚本测试成功")


def crawler():
    headers = {
        'Host': 'www.cwl.gov.cn',
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:84.0) Gecko/20100101 Firefox/84.0',
        'Referer': 'http://www.cwl.gov.cn/kjxx/ssq/kjgg/'
    }
    req = get('http://www.cwl.gov.cn/cwl_admin/kjxx/findDrawNotice?name=ssq&issueCount=&issueStart=&issueEnd=&dayStart=2013-01-08&dayEnd=2020-12-31&pageNo=1', headers=headers)
    req.encoding = 'utf-8'
    data = req.json()['result'][0]
    db = connect(host='cdb-geebna1a.cd.tencentcdb.com', port=10053, user='root', passwd='HXhlx19990627', database='lorrety')
    cursor = db.cursor()
    s = ''
    for p in data['prizegrades']:
        if p['type'] != 7:
            s += str(p['type']) + ':' + p['typenum'] + ':' + p['typemoney'] + ','
    cursor.execute('insert into DoubleChromosphere values (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)', (data['code'], data['date'][:10], data['week'], data['red'], data['blue'], data['sales'], data['poolmoney'], data['content'], s[:-1], data['detailsLink'], data['videoLink']))
    db.commit()
