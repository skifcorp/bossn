#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <QString>

class QDomDocument;

class MrwSettings
{
	public:
		// instance - Возвращает  ЕЬИНСТВЕННЫЙ ексземпляр  MrwSettings ( Петтерт ОДИНОЧКА)
		static MrwSettings* instance();
		void print();
		// load - Загружает из XML файла fileName данные
		bool load( const QString& fileName );
		
		int     platformaWeightAddress[2];
		QString platformaWeightType[2];
		QString platformaWeightPort[2];
		int     platformaWeightMode[2];
		int     platformaWeightTimeOut[2];
		int     platformaReaderAddressTablo[2];
		int     platformaReaderAddressReader[2];
		int     platformaPerimetrTestDIO[2];
		int     platformaPerimetrEnterSensDI[2];
		int     platformaPerimetrExitSensDI[2];
		int     platformaTrafficLightRed[2];
		int     platformaTrafficLightYellow[2];
		int     platformaTrafficLightGreen[2];
		int     platformaOff[2];
		
		QString busRegistratorPort;
		int busRegistratorTimeOut;
		
		int commonWeightZero;
		int commonDiTimeOut;
		int commonPerTimeOut;
		int commonWaitNewCardTimeOut;
		int commonMaxDiffBetwReweight;
		int commonDeviationFromNormTara;
		int commonPerimeterAlgorithm;
		int commonMinWeightOfCar;
		int commonNeedRestart;
		int commonKindOfWeight;
		int commonUseRezField;
		int commonAlgorithmOfAnalysis;
		int commonSizeOfGroup;
		int commonNumberFromGroup;
		int commonUpLimit;
		int commonDownLimit;
		
		QString dataBaseControllerHost;
		QString dataBaseControllerLogin;
		QString dataBaseControllerPassword;
		QString dataBaseServerHost;
		QString dataBaseServerLogin;
		QString dataBaseServerPassword;
		QString dataBaseLogPath;
		QString dataBaseFlagPath;
	
	protected:
		MrwSettings();
	
	private:
		bool parse(const QDomDocument& doc);
		static MrwSettings* _Instance;
};

#endif
