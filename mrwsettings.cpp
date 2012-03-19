#include "mrwsettings.h"
#include <QDomDocument>

#include <QFile>

MrwSettings* MrwSettings::_Instance = (MrwSettings*)0;

MrwSettings* MrwSettings::instance()
{
	if (_Instance == 0)
	{
		_Instance = new MrwSettings;
	}
	return _Instance;
}

MrwSettings::MrwSettings()
{
}

bool MrwSettings::load( const QString& fileName )
{
	QFile file( fileName );
	if ( !file.open(QIODevice::ReadOnly) ) return false;
	QDomDocument doc;
	doc.setContent( &file );
	return parse( doc );
}

bool MrwSettings::parse( const QDomDocument& doc )
{
	QDomElement doc_element = doc.documentElement();
	QDomElement header_node = doc_element.firstChild().toElement();
	if ( header_node.isNull() )
	{
		qDebug ("func node is empty!!!");
		return false;
	}
	
	while (!header_node.isNull() )
	{
		//qDebug("header_node =%s", qPrintable(header_node.tagName()));
		
		if ((header_node.tagName() == "platforma1") || (header_node.tagName() == "platforma2"))
		{
			int index=0;
			if (header_node.tagName() == "platforma1") index=0;
				else if (header_node.tagName() == "platforma2") index=1;
					else return false;
			
			QDomElement platformaSettNode = header_node.firstChild().toElement();
			while ( !platformaSettNode.isNull() )
			{
				if ( platformaSettNode.tagName() == "weight")
				{
					QDomElement weightSettNode = platformaSettNode.firstChild().toElement();
					while ( !weightSettNode.isNull() )
					{
						if ( weightSettNode.tagName() == "address") platformaWeightAddress[index]=weightSettNode.text().toInt();
						if ( weightSettNode.tagName() == "type") platformaWeightType[index]=weightSettNode.text();
						if ( weightSettNode.tagName() == "port") platformaWeightPort[index]=weightSettNode.text();
						if ( weightSettNode.tagName() == "mode") platformaWeightMode[index]=weightSettNode.text().toInt();
						if ( weightSettNode.tagName() == "timeout") platformaWeightTimeOut[index]=weightSettNode.text().toInt();
						weightSettNode = weightSettNode.nextSibling().toElement();
					}
				}
				
				if ( platformaSettNode.tagName() == "reader")
				{
					QDomElement readerSettNode = platformaSettNode.firstChild().toElement();
					while ( !readerSettNode.isNull() )
					{
						if ( readerSettNode.tagName() == "addressTablo") platformaReaderAddressTablo[index]=readerSettNode.text().toInt();
						if ( readerSettNode.tagName() == "addressReader") platformaReaderAddressReader[index]=readerSettNode.text().toInt();
						readerSettNode = readerSettNode.nextSibling().toElement();
					}
				}
				
				if ( platformaSettNode.tagName() == "perimetr")
				{
					QDomElement perimetrSettNode = platformaSettNode.firstChild().toElement();
					while ( !perimetrSettNode.isNull() )
					{
						if ( perimetrSettNode.tagName() == "testDIO") platformaPerimetrTestDIO[index]=perimetrSettNode.text().toInt();
						if ( perimetrSettNode.tagName() == "enterSensDI") platformaPerimetrEnterSensDI[index]=perimetrSettNode.text().toInt();
						if ( perimetrSettNode.tagName() == "exitSensDI") platformaPerimetrExitSensDI[index]=perimetrSettNode.text().toInt();
						perimetrSettNode = perimetrSettNode.nextSibling().toElement();
					}
				}
				
				if ( platformaSettNode.tagName() == "trafficLight")
				{
					QDomElement trafficLightSettNode = platformaSettNode.firstChild().toElement();
					while ( !trafficLightSettNode.isNull() )
					{
						if ( trafficLightSettNode.tagName() == "red") platformaTrafficLightRed[index]=trafficLightSettNode.text().toInt();
						if ( trafficLightSettNode.tagName() == "yellow") platformaTrafficLightYellow[index]=trafficLightSettNode.text().toInt();
						if ( trafficLightSettNode.tagName() == "green") platformaTrafficLightGreen[index]=trafficLightSettNode.text().toInt();
						trafficLightSettNode = trafficLightSettNode.nextSibling().toElement();
					}
				}
				
				if (platformaSettNode.tagName() == "off") platformaOff[index]=platformaSettNode.text().toInt();
				
				platformaSettNode = platformaSettNode.nextSibling().toElement();
			}
		}
		
		if (header_node.tagName() == "busRegistrator")
		{
			QDomElement busRegistratorSettNode = header_node.firstChild().toElement();
			while ( !busRegistratorSettNode.isNull() )
			{
				if ( busRegistratorSettNode.tagName() == "port") busRegistratorPort=busRegistratorSettNode.text();
				if ( busRegistratorSettNode.tagName() == "timeout") busRegistratorTimeOut=busRegistratorSettNode.text().toInt();
				busRegistratorSettNode = busRegistratorSettNode.nextSibling().toElement();
			}
		}
		
		if (header_node.tagName() == "common")
		{
			QDomElement commonSettNode = header_node.firstChild().toElement();
			while ( !commonSettNode.isNull() )
			{
				if ( commonSettNode.tagName() == "weightZero") commonWeightZero=commonSettNode.text().toInt();
				if ( commonSettNode.tagName() == "diTimeOut") commonDiTimeOut=commonSettNode.text().toInt();
				if ( commonSettNode.tagName() == "perTimeOut") commonPerTimeOut=commonSettNode.text().toInt();
				if ( commonSettNode.tagName() == "waitNewCardTimeOut") commonWaitNewCardTimeOut=commonSettNode.text().toInt();
				if ( commonSettNode.tagName() == "maxDiffBetwReweight") commonMaxDiffBetwReweight=commonSettNode.text().toInt();
				if ( commonSettNode.tagName() == "deviationFromNormTara") commonDeviationFromNormTara=commonSettNode.text().toInt();
				if ( commonSettNode.tagName() == "perimeterAlgorithm") commonPerimeterAlgorithm=commonSettNode.text().toInt();
				if ( commonSettNode.tagName() == "minWeightOfCar") commonMinWeightOfCar=commonSettNode.text().toInt();
				if ( commonSettNode.tagName() == "needRestart") commonNeedRestart=commonSettNode.text().toInt();
				if ( commonSettNode.tagName() == "kindOfWeight") commonKindOfWeight=commonSettNode.text().toInt();
				if ( commonSettNode.tagName() == "useRezField") commonUseRezField=commonSettNode.text().toInt();
				if ( commonSettNode.tagName() == "algorithmOfAnalysis") commonAlgorithmOfAnalysis=commonSettNode.text().toInt();
				if ( commonSettNode.tagName() == "sizeOfGroup") commonSizeOfGroup=commonSettNode.text().toInt();
				if ( commonSettNode.tagName() == "numberFromGroup") commonNumberFromGroup=commonSettNode.text().toInt();
				if ( commonSettNode.tagName() == "upLimit") commonUpLimit=commonSettNode.text().toInt();
				if ( commonSettNode.tagName() == "downLimit") commonDownLimit=commonSettNode.text().toInt();
				commonSettNode = commonSettNode.nextSibling().toElement();
			}
		}
		
		if (header_node.tagName() == "database")
		{
			QDomElement dataBaseSettNode = header_node.firstChild().toElement();
			while ( !dataBaseSettNode.isNull() )
			{
				if ( dataBaseSettNode.tagName() == "controller")
				{
					QDomElement controllerSettNode = dataBaseSettNode.firstChild().toElement();
					while ( !controllerSettNode.isNull() )
					{
						if ( controllerSettNode.tagName() == "host") dataBaseControllerHost=controllerSettNode.text();
						if ( controllerSettNode.tagName() == "login") dataBaseControllerLogin=controllerSettNode.text();
						if ( controllerSettNode.tagName() == "password") dataBaseControllerPassword=controllerSettNode.text();
						controllerSettNode = controllerSettNode.nextSibling().toElement();
					}
				}
				
				if ( dataBaseSettNode.tagName() == "server")
				{
					QDomElement serverSettNode = dataBaseSettNode.firstChild().toElement();
					while ( !serverSettNode.isNull() )
					{
						if ( serverSettNode.tagName() == "host") dataBaseServerHost=serverSettNode.text();
						if ( serverSettNode.tagName() == "login") dataBaseServerLogin=serverSettNode.text();
						if ( serverSettNode.tagName() == "password") dataBaseServerPassword=serverSettNode.text();
						serverSettNode = serverSettNode.nextSibling().toElement();
					}
				}
				
				if ( dataBaseSettNode.tagName() == "logPath") dataBaseLogPath=dataBaseSettNode.text();
				if ( dataBaseSettNode.tagName() == "flagPath") dataBaseFlagPath=dataBaseSettNode.text();
				
				dataBaseSettNode = dataBaseSettNode.nextSibling().toElement();
			}
		}
		
		header_node = header_node.nextSibling().toElement();
	}
	return true;
}

void MrwSettings::print()
{
	qDebug("MrwSettings:");
	
	for (int i=0; i<2; i++)
	{
		qDebug(qPrintable("platforma"+QString::number(i+1)));
		qDebug("	weight");
		qDebug("		address = %d", platformaWeightAddress[i]);
		qDebug("		type = %s", qPrintable(platformaWeightType[i]));
		qDebug("		port = %s", qPrintable(platformaWeightPort[i]));
		qDebug("		mode = %d", platformaWeightMode[i]);
		qDebug("		timeout = %d", platformaWeightTimeOut[i]);
		qDebug("	reader");
		qDebug("		addressTablo = %d", platformaReaderAddressTablo[i]);
		qDebug("		addressReader = %d", platformaReaderAddressReader[i]);
		qDebug("	perimetr");
		qDebug("		testDIO = %d", platformaPerimetrTestDIO[i]);
		qDebug("		enterSensDI = %d", platformaPerimetrEnterSensDI[i]);
		qDebug("		exitSensDI = %d", platformaPerimetrExitSensDI[i]);
		qDebug("	trafficLight");
		qDebug("		red = %d", platformaTrafficLightRed[i]);
		qDebug("		yellow = %d", platformaTrafficLightYellow[i]);
		qDebug("		green = %d", platformaTrafficLightGreen[i]);
		qDebug("	off = %d", platformaOff[i]);
	}
	
	qDebug("busRegistrator");
	qDebug("	port = %s", qPrintable(busRegistratorPort));
	qDebug("	timeout = %d", busRegistratorTimeOut);
	qDebug("common");
	qDebug("	weightZero = %d", commonWeightZero);
	qDebug("	diTimeOut = %d", commonDiTimeOut);
	qDebug("	perTimeOut = %d", commonPerTimeOut);
	qDebug("	waitNewCardTimeOut = %d", commonWaitNewCardTimeOut);
	qDebug("	maxDiffBetwReweight = %d", commonMaxDiffBetwReweight);
	qDebug("	deviationFromNormTara = %d", commonDeviationFromNormTara);
	qDebug("	perimeterAlgorithm = %d", commonPerimeterAlgorithm);
	qDebug("	minWeightOfCar = %d", commonMinWeightOfCar);
	qDebug("	needRestart = %d", commonNeedRestart);
	qDebug("	kindOfWeight = %d", commonKindOfWeight);
	qDebug("	useRezField = %d", commonUseRezField);
	qDebug("	algorithmOfAnalysis = %d", commonAlgorithmOfAnalysis);
	qDebug("	sizeOfGroup = %d", commonSizeOfGroup);
	qDebug("	numberFromGroup = %d", commonNumberFromGroup);
	qDebug("	upLimit = %d", commonUpLimit);
	qDebug("	downLimit = %d", commonDownLimit);
	qDebug("database");
	qDebug("	controller");
	qDebug("		host = %s", qPrintable(dataBaseControllerHost));
	qDebug("		login = %s", qPrintable(dataBaseControllerLogin));
	qDebug("		password = %s", qPrintable(dataBaseControllerPassword));
	qDebug("	server");
	qDebug("		host = %s", qPrintable(dataBaseServerHost));
	qDebug("		login = %s", qPrintable(dataBaseServerLogin));
	qDebug("		password = %s", qPrintable(dataBaseServerPassword));
	qDebug("	logPath = %s", qPrintable(dataBaseLogPath));
	qDebug("	flagPath = %s", qPrintable(dataBaseFlagPath));
}
