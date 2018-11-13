#include "PatientInfo.h"

CPatientInfo::CPatientInfo(QWidget * parent): CBaseFunWidget(parent)
{
    m_iFlag = 0;
    m_bIdListHide = false;
    m_bSampleTypeListHide = false;
    m_pSyszux = new SyszuxPinyin();
    connect(m_pSyszux,SIGNAL(sendPinyin(QString)),this,SLOT(ConfirmString(QString)));

    m_card = NULL;
    m_nameCell = NULL;

    InitUI();

    timerBtnConfirm = new QTimer();
    connect(timerBtnConfirm,SIGNAL(timeout()), this,SLOT(timeoutBtnConfirm()));
}

void CPatientInfo::showEvent(QShowEvent *)
{
    m_pXbtConfirm->setEnabled(false);
    timerBtnConfirm->start(4000);
}

void CPatientInfo::closeEvent(QCloseEvent *)
{
    ClearUI();
}

void CPatientInfo::InitUI()
{
    QFont font;
    font.setPointSize(18);
    font.setFamily(QObject::tr("wenquanyi"));

    QPalette lette;
    lette.setColor(QPalette::Base, QColor(Qt::gray));

    m_pLbTitle = new XLabel(this);
    //m_pLbTitle->setText(tr("患者信息"));
    m_pLbTitle->setText(tr("Patient information"));
    m_pLbTitle->setGeometry(0, 80, 800, 40);
    m_pLbTitle->setAlignment(Qt::AlignCenter);

    m_pLbID = new XLabel(this);
    m_pLbID->setText(tr("ID："));
    m_pLbID->setGeometry(130, 150, 40, 40);
    m_pLbID->setStyleSheet(FONTFAMILY + FONT_SIZE_3);

    m_pLeID = new CMyLineEdit(this);
    m_pLeID->SetMyGeometry(190,150,180,40);
    m_pLeID->setPalette(lette);
    m_pLeID->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_pLeID,SIGNAL(SendText(QString)),this,SLOT(CallKeyBoard(QString)));
    connect(m_pLeID,SIGNAL(textChanged(QString)),this,SLOT(IdEditFinished()));

    m_pBtID = new QPushButton(this);
    m_pBtID->setText(tr("▼"));
    m_pBtID->setGeometry(370,150,16,40);
    //m_pBtID->setStyleSheet("QPushButton{border-radius:0px}");
    connect(m_pBtID,SIGNAL(clicked()),this,SLOT(BtIdListClicked()));

    m_pLbName = new XLabel(this);
    //m_pLbName->setText(tr("姓名："));
    m_pLbName->setText(tr("Name:"));
    m_pLbName->setGeometry(130, 210, 80, 40);
    m_pLbName->setStyleSheet(FONTFAMILY + FONT_SIZE_3);

    m_pLeName = new CMyLineEdit(this);
    m_pLeName->SetMyGeometry(210,210,120,40);
    m_pLeName->setPalette(lette);
    m_pLeName->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_pLeName,SIGNAL(SendText(QString)),this,SLOT(CallKeyBoard(QString)));

    m_pLbSex = new XLabel(this);
    //m_pLbSex->setText(tr("性别："));
    m_pLbSex->setText(tr("Sex:"));
    m_pLbSex->setGeometry(360, 210, 60, 40);
    m_pLbSex->setStyleSheet(FONTFAMILY + FONT_SIZE_3);

    m_pCbbSex = new QComboBox(this);
    m_pCbbSex->setGeometry(422, 210, 100, 40);
    m_pCbbSex->insertItem(0,tr("Male"));
    m_pCbbSex->insertItem(1,tr("Female"));
    m_pCbbSex->setStyleSheet(FONTFAMILY + FONT_SIZE_6);

    m_pLbAge = new XLabel(this);
    m_pLbAge->setText(tr("Age:"));
    m_pLbAge->setGeometry(530, 210, 60, 40);
    m_pLbAge->setStyleSheet(FONTFAMILY + FONT_SIZE_3);

    m_pValidator = new QIntValidator(0,200,this);
    m_pLeAge = new CMyLineEdit(this);
    m_pLeAge->setValidator(m_pValidator);
    m_pLeAge->SetMyGeometry(600,210,80,40);
    m_pLeAge->setPalette(lette);
    m_pLeAge->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_pLeAge,SIGNAL(SendText(QString)),this,SLOT(CallKeyBoard(QString)));

    m_pLbSampleNo= new XLabel(this);
    m_pLbSampleNo->setText(tr("Sample SN:"));
    m_pLbSampleNo->setGeometry(130, 270, 130, 40);
    m_pLbSampleNo->setStyleSheet(FONTFAMILY + FONT_SIZE_3);

    m_pLeSampleNo = new CMyLineEdit(this);
    m_pLeSampleNo->SetMyGeometry(260,270,150,40);
    m_pLeSampleNo->setPalette(lette);
    m_pLeSampleNo->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_pLeSampleNo,SIGNAL(SendText(QString)),this,SLOT(CallKeyBoard(QString)));

    m_pLbSampleType = new XLabel(this);
    m_pLbSampleType->setText(tr("Sample:"));
    m_pLbSampleType->setGeometry(420, 270, 110, 40);
    m_pLbSampleType->setStyleSheet(FONTFAMILY + FONT_SIZE_3);

    m_pCbbSampleType = new QComboBox(this);
    m_pCbbSampleType->setGeometry(530,270,150,40);
    m_pCbbSampleType->insertItem(0,tr("Serum"));
    m_pCbbSampleType->insertItem(1,tr("Plasma"));
    m_pCbbSampleType->insertItem(2,tr("Whole blood"));
    m_pCbbSampleType->insertItem(3,tr("Urine"));
    m_pCbbSampleType->insertItem(4,tr("Others"));
    m_pCbbSampleType->setPalette(lette);
    m_pCbbSampleType->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    m_pCbbSampleType->setAutoFillBackground(true);

    m_pXbtConfirm = new XButton(this);
    m_pXbtConfirm->setGeometry(360, 360, 100, 40);
    m_pXbtConfirm->setText(tr("Confirm"));
    connect(m_pXbtConfirm,SIGNAL(sigbtnclick()),this,SLOT(BtConfirmClicked()));

    m_pXbtReturn = new XButton(this);
    m_pXbtReturn->setGeometry(520, 360, 100, 40);
    m_pXbtReturn->setText(tr("Back"));
    connect(m_pXbtReturn,SIGNAL(sigbtnclick()),this,SLOT(BtReturnClicked()));

    m_pIdListWidget = new QListWidget(this);
    m_pIdListWidget->setFont(font);
    m_pIdListWidget->hide();
    connect(m_pIdListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(IdItemListClicked(QListWidgetItem*)));
}

void CPatientInfo::ClearUI()
{
    m_pLeID->clear();
    m_pLeName->clear();
    m_pLeAge->clear();
    m_pLeSampleNo->clear();
    m_pCbbSampleType->setCurrentIndex(0);
}

void CPatientInfo::SetAutoID(QString qStrID)
{
    m_pLeID->clear();
    m_pLeName->clear();
    m_pLeAge->clear();
    m_pLeSampleNo->clear();
    m_pCbbSampleType->setCurrentIndex(0);

    m_pLeID->setText(qStrID);
}

void CPatientInfo::SetSerNum(int iSerNum)
{
    m_iSerNum = iSerNum;
}

void CPatientInfo::ShowData(QString qStrID, QString qStrName, QString qStrSex, QString qStrAge, QString qStrSmpNum, QString qStrSmpType)
{
    m_pLeID->setText(qStrID);
    m_pLeName->setText(qStrName);
    if(qStrSex.compare(tr("Male")) == 0)
    {
        m_pCbbSex->setCurrentIndex(0);
    }
    else if(qStrSex.compare(tr("Female")) == 0)
    {
        m_pCbbSex->setCurrentIndex(1);
    }
    else
    {
        m_pCbbSex->setCurrentIndex(0);
    }

    m_pLeAge->setText(qStrAge);
    m_pLeSampleNo->setText(qStrSmpNum);

    if(qStrSmpType.compare(tr("Serum")) == 0)
    {
        m_pCbbSampleType->setCurrentIndex(0);
    }
    else if(qStrSmpType.compare(tr("Plasma")) == 0)
    {
        m_pCbbSampleType->setCurrentIndex(1);
    }
    else if(qStrSmpType.compare(tr("Whole blood")) == 0)
    {
        m_pCbbSampleType->setCurrentIndex(2);
    }
    else if(qStrSmpType.compare(tr("Urine")) == 0)
    {
        m_pCbbSampleType->setCurrentIndex(3);
    }
    else if(qStrSmpType.compare(tr("Others")) == 0)
    {
        m_pCbbSampleType->setCurrentIndex(4);
    }
    else
    {
        m_pCbbSampleType->setCurrentIndex(0);
    }

}

CPatientInfo::~CPatientInfo()
{
    delete m_pLbTitle;
    delete m_pLbID;
    delete m_pLeID;
    delete m_pLbName;
    delete m_pLeName;
    delete m_pLbSex;
    delete m_pCbbSex;
    delete m_pLbAge;
    delete m_pLeAge;
    delete m_pLbSampleNo;
    delete m_pLeSampleNo;
    delete m_pLbSampleType;
    delete m_pXbtConfirm;
    delete m_pXbtReturn;
    delete m_pSyszux;
}

void CPatientInfo::CallKeyBoard(QString qStrText)
{
    printf("CallKeyBoard!\n");
    if(sender() == (QObject *)m_pLeID)
    {
        m_pSyszux->SetInputMethodEng();
        m_iFlag = ID;
    }
    else if(sender() == (QObject *)m_pLeName)
    {
        m_pSyszux->SetInputMethodEng();
        m_iFlag = NAME;
    }
    else if(sender() == (QObject *)m_pLeAge)
    {
        m_pSyszux->SetInputMethodEng();
        m_iFlag = AGE;
    }
    else if(sender() == (QObject *)m_pLeSampleNo)
    {
        m_pSyszux->SetInputMethodEng();
        m_iFlag = SAMPLENUM;
    }
    else
    {
        printf("Error\n");
    }

    m_pSyszux->lineEdit_window->setText(qStrText);
    m_pSyszux->show();
}

void CPatientInfo::ConfirmString(QString qStrText)
{
    switch (m_iFlag)
    {
    case ID:
        m_pLeID->setText(qStrText);
        break;
    case NAME:
        m_pLeName->setText(qStrText);
        break;
    case AGE:
        m_pLeAge->setText(qStrText);
        break;
    case SAMPLENUM:
        m_pLeSampleNo->setText(qStrText);
        break;
    default:
        break;
    }
}

void CPatientInfo::BtConfirmClicked()
{
    QString strID = m_pLeID->text().trimmed();
    QString strName = m_pLeName->text().trimmed();
    QString strAge = m_pLeAge->text().trimmed();
    QString strSampleNO = m_pLeSampleNo->text().trimmed();

    if (strID.length() <= 0
            && strName.length() <= 0
            && strAge.length() <= 0
            && strSampleNO.length() <= 0)
        return;

    qDebug()<<"------In patientInfo SerNum:"<<m_iSerNum;

    //保存数据库 TABLE_BODY
    patientInfoList.clear();
    patientInfoList.append(QString::number(m_iSerNum));
    patientInfoList.append(strID);
    patientInfoList.append(strName);
    patientInfoList.append(m_pCbbSex->currentText());
    patientInfoList.append(strAge);
    patientInfoList.append(strSampleNO);
    patientInfoList.append(m_pCbbSampleType->currentText());

    bool isPatientExist = CDataBase::IsPatientExist(QString::number(m_iSerNum));
    if(isPatientExist)
        CDataBase::UpdataPatientInfo(patientInfoList);
    else
        if(m_iSerNum)
            CDataBase::add_Record(PATIENT_BODY_NAME,patientInfoList);

    if (m_card) {
        m_card->setnotes(strID,
                         strName,
                         m_pCbbSex->currentText(),
                         strAge,
                         strSampleNO,
                         m_pCbbSampleType->currentText());

        if(m_card->gettestend()) {
            int dataId = m_card->getsavepos();
            QSqlQuery query = CDataBase::select_Record(TABLE_BODY_NAME,"SERNUM",m_card->getsavepos());

            if(query.next() && strID != query.value(23).toString()) {
                QDateTime cur = QDateTime::currentDateTime();
                CDataBase::updata_Record(TABLE_BODY_NAME,"NOTES","SERNUM", strID, dataId);
                CDataBase::updata_Record(TABLE_BODY_NAME,"UPDATED","SERNUM", cur, dataId);
                m_card->setupdatedtime(cur);
            }
        }
    }

    if (m_nameCell) {
        m_nameCell->setText(strName);
    }

    emit EditFinish(strID, m_card);

    this->hide();
}

void CPatientInfo::BtReturnClicked()
{
    this->hide();
    /*
    bSave = false;
    patientInfoList.clear();

    CI::get_insertcard()->monitor(false);
    //CI::get_detection()->takeOutCard();
    CI::get_qrscan()->clearScanRecord();
    CI::set_btiming(false);
    emit sigperform(MODE_WG);
    */
}

void CPatientInfo::IndexChange(int iIndex)
{
    if(iIndex == 4)
    {
        CallKeyBoard(tr(""));
    }
}

void CPatientInfo::ComboboxEdit(QString qStrText)
{
    CallKeyBoard(qStrText);
}

void CPatientInfo::BtIdListClicked()
{
    QString qStrID;
    if(m_bIdListHide == true)
    {
        m_bIdListHide = false;
        m_pIdListWidget->hide();
    }
    else
    {
        m_pIdListWidget->clear();
        m_pIdListWidget->setGeometry(210,190,180,110);
        m_pIdListWidget->setStyleSheet("color:rgb(0, 0, 0);" + FONTFAMILY + FONT_SIZE_4);
        QSqlQuery query = CDataBase::GetPatientList();
        for(int i=0;i<3;i++)
        {
            if(query.next())
            {
                qStrID = query.value(0).toString();
                m_pIdListWidget->addItem(qStrID);
            }
        }
        m_pIdListWidget->show();
        m_bIdListHide = true;
    }
}

void CPatientInfo::IdItemListClicked(QListWidgetItem *item)
{
    m_pLeID->setText(item->text());
    m_pIdListWidget->hide();
    m_bIdListHide = false;
}

void CPatientInfo::IdEditFinished()
{
    QString qStrName;
    QString qStrSex;
    QString qStrAge;
    QString qStrID = m_pLeID->text();
    QSqlQuery query = CDataBase::GetPatientInfoByID(qStrID);
    if(query.next())
    {
        qStrName = query.value(2).toString();
        qStrSex = query.value(3).toString();
        qStrAge = query.value(4).toString();

        m_pLeName->setText(qStrName);
        if(qStrSex.compare(tr("Male")) == 0)
        {
            m_pCbbSex->setCurrentIndex(0);
        }
        else
        {
            m_pCbbSex->setCurrentIndex(1);
        }
        m_pLeAge->setText(qStrAge);
    }
}

void CPatientInfo::setCurrEditCtrl(XTestCard *card, QTableWidgetItem *nameCell)
{
    m_card = card;
    m_nameCell = nameCell;
}

void CPatientInfo::hideEvent(QHideEvent *)
{
    m_card = NULL;
    m_nameCell = NULL;
}

void CPatientInfo::timeoutBtnConfirm()
{
    timerBtnConfirm->stop();
    m_pXbtConfirm->setEnabled(true);
}


