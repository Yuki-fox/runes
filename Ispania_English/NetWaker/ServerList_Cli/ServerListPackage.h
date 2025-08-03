#include "ServerListPackageDef.h"
// S = ServerList
// SC = Game Srv ( �ثe�� Switch Srv )
// C = Client

namespace   NetWakerServerListPackage
{
	//--------------------------------------------------------------------------------------    
	//�Ũt�Ϋʥ]
	struct ServerListNet_SysNull
	{
		ServerListPacketCommandEnum Command;
	};
	//-------------------------------------------------------------------------------------
	//SC -> S ���U�ثe�u�W�H��
	struct ServerListNet_SCtoS_RegServerInfo
	{
		ServerListPacketCommandEnum Command;
		char	ServerName[ _DEF_MAX_GSRVNAME_SIZE_ ];
		char	IP_DNS[ _DEF_MAX_IPSTR_SIZE_ ];
		int		Port;
		int		MaxPlayerCount;		//�̤j�i�W�u�H��
		int		ParallelZoneCount;
		int		PlayerCount[100];
		int		GameID;
		int		ServerStatus;
		float	TotalPlayerRate;

		ServerListNet_SCtoS_RegServerInfo( )
		{
			Command = EM_ServerListNet_SCtoS_RegServerInfo;
		}
	};
	//-------------------------------------------------------------------------------------
	//C -> S �n�DServerList ���
	struct ServerListNet_CtoS_ServerListRequest
	{
		ServerListPacketCommandEnum Command;
		char    Account[ _DEF_MAX_ACCOUNTNAME_SIZE_ ];
		char    Password[ _DEF_MAX_PASSWORD_SIZE_ ];
		bool	KickAccount;
		bool	HashedPassword;

		ServerListNet_CtoS_ServerListRequest( )
		{
			Command = EM_ServerListNet_CtoS_ServerListRequest;
		}
	};
	//-------------------------------------------------------------------------------------
	//S -> C ServerList���
	struct ServerListNet_StoC_ServerListResult
	{
		ServerListPacketCommandEnum Command;

		int			TotalCount;
		int			Index;

		char		ServerName[ _DEF_MAX_GSRVNAME_SIZE_ ];
		char		IP_DNS[ _DEF_MAX_IPSTR_SIZE_ ];
		int			Port;
		int			ParallelZoneCount;			//����Ŷ��ƶq
		float		PlayerRate[100];			//0(��)~1(��)
		int			UpdateTime;					//�h�[�e��s��
		int			WorldID;
		int			LastLoginWorldID;			//
		char    	Account[ _DEF_MAX_ACCOUNTNAME_SIZE_ ];
		unsigned	IPNum;						
		char		Country[3];					//��a�Y�g

		int			ServerStatus;				//0 �����`, 1 ������, 2 ������
		int			ServerlistMode;				//0 ���`, 1 �����
		float		TotalPlayerRate;
		int			Age;
		int			ValidTime;
		int			VivoxLicenseTime;

		ServerListNet_StoC_ServerListResult( )
		{
			Command = EM_ServerListNet_StoC_ServerListResult;
		}
	};
	//-------------------------------------------------------------------------------------
	enum ServerListFailed_ENUM
	{
		EM_ServerListFailed_Unknow			= -1,
		EM_ServerListFailed_RoleInWorld		,		//�����٦b�C����
		EM_ServerListFailed_Password_Error	,		//�b���αK�X�O����
		EM_ServerListFailed_NoWorld			,		//�S���@�ӥ@�ɬO�}��
		EM_ServerListFailed_ValidTimeError	,		//�b�����Įɶ�
		EM_ServerListFailed_IPBlock			,		//���ϰ�IP����n�J
		EM_ServerListFailed_CommLock		,		//�q�T����w��

		EM_ServerListFailed_Freeze_Self		= 100,	//�ۤv�b���N��
		EM_ServerListFailed_Freeze_Official	,		//�x��b���N��
		EM_ServerListFailed_Freeze_Other3	,		//�b���N��
		EM_ServerListFailed_Freeze_Other4	,		//�b���N��
		EM_ServerListFailed_Freeze_Other5	,		//�b���N��
		EM_ServerListFailed_Freeze_Other6	,		//�b���N��
		EM_ServerListFailed_Freeze_Other7	,		//�b���N��
		EM_ServerListFailed_Freeze_Other8	,		//�b���N��
		EM_ServerListFailed_Freeze_Other9	,		//�b���N��

		EM_ServerListFailed_BetaKeyRequest  = 200,	//k2�ݭnbeta 
		EM_ServerListFailed_Gama_Wrong_OTP		,	//��l���~ Wrong_OTP : One time password is not correct
		EM_ServerListFailed_Gama_Wrong_EventOTP	,
		EM_ServerListFailed_VN_Wrong_UserState	,	//�V�n�n�J���~�T�� �b���S���{��
		EM_ServerListFailed_Gama_Wrong_Timeout	,
		EM_ServerListFailed_Gama_NoAccount		,	//�b�����s�b
		EM_ServerListFailed_KR_AgeRestriction	,	//����n�J - �~�֭���

		EM_ServerListFailed_ConnectFailed	= 300	// �s�u����, ���ˬd�����Ψ�����]�w
	};
	//S -> C �n�DServerList ��ƥ���
	struct ServerListNet_StoC_ServerListFailed
	{
		ServerListPacketCommandEnum Command;
		ServerListFailed_ENUM Result;

		ServerListNet_StoC_ServerListFailed( )
		{
			Command = EM_ServerListNet_StoC_ServerListFailed;
		}
	};
	//-------------------------------------------------------------------------------------
	// S -> SC ��b�����_�n�J�q��Switch
	struct ServerListNet_StoSC_CliReloginNotify
	{
		ServerListPacketCommandEnum Command;
		char	Account[_DEF_MAX_ACCOUNTNAME_SIZE_];

		ServerListNet_StoSC_CliReloginNotify( )
		{
			Command = EM_ServerListNet_StoSC_CliReloginNotify;
		}
	};
	//-------------------------------------------------------------------------------------
	// S -> C �q���b���b�UServer�����H��
	struct ServerListNet_StoC_GameRoleCount
	{
		ServerListPacketCommandEnum Command;
		char		RoleCount[30];
		int			DataSize;
		char		FileData[0x10000];

		ServerListNet_StoC_GameRoleCount( )
		{
			Command = EM_ServerListNet_StoC_GameRoleCount;
		}
		int Size()
		{
			return sizeof( *this ) - sizeof( FileData ) + DataSize;
		}
	};
   	//-------------------------------------------------------------------------------------
	//�n�D��e�ʥ]
	struct ServerListNet_CtoS_ForwardTo
	{
		ServerListPacketCommandEnum Command;

		//int		IPNum;
		char	IPStr[ _DEF_MAX_IPSTR_SIZE_ ];
		int		Port;

		ServerListNet_CtoS_ForwardTo( )
		{
			Command = EM_ServerListNet_CtoS_ForwardTo;
		}
	};
	//-------------------------------------------------------------------------------------
	//�g�JVivox 
	struct ServerListNet_CtoS_SetVivoxLicenseTime
	{
		ServerListPacketCommandEnum Command;

		ServerListNet_CtoS_SetVivoxLicenseTime( )
		{
			Command = EM_ServerListNet_CtoS_SetVivoxLicenseTime;
		}
	};	
	//-------------------------------------------------------------------------------------
	// S -> C �n�D Client ���m�K�X
	struct ServerListNet_StoC_ResetPassword
	{
		ServerListPacketCommandEnum Command;

		ServerListNet_StoC_ResetPassword( )
		{
			Command = EM_ServerListNet_StoC_ResetPassword;
		}
	};

	//-------------------------------------------------------------------------------------
	// C -> S Client ���m�K�X
	struct ServerListNet_CtoS_ResetPasswordResult
	{
		ServerListPacketCommandEnum Command;

		//char    Account[ 64 ];
		char    Account[ _DEF_MAX_ACCOUNTNAME_SIZE_ ];
		char    OldPassword[ _DEF_MAX_PASSWORD_SIZE_ ];
		char    NewPassword[ _DEF_MAX_PASSWORD_SIZE_ ];

		ServerListNet_CtoS_ResetPasswordResult( )
		{
			Command = EM_ServerListNet_CtoS_ResetPasswordResult;
		}
	};

	//-------------------------------------------------------------------------------------
	// S -> C �]�m�K�X���G
	struct ServerListNet_StoC_ResetPasswordResult
	{
		ServerListPacketCommandEnum Command;

		int Result;	

		ServerListNet_StoC_ResetPasswordResult( )
		{
			Command = EM_ServerListNet_StoC_ResetPasswordResult;
		}
	};

	//-------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//S -> C �n�DServerList ��ƥ��� �B�~���
	union ServerListFailedDataUnion
	{
		struct
		{
			int Count;
			int Max;
			int FreezeTime;
		}PasswordError;

		struct
		{
			int RemainingTime;
		}Freeze;
	};

	struct ServerListNet_StoC_ServerListFailedData
	{
		ServerListPacketCommandEnum Command;
		ServerListFailed_ENUM		Result;
		ServerListFailedDataUnion	Data;

		ServerListNet_StoC_ServerListFailedData( )
		{
			memset(this, 0, sizeof(*this));
			Command = EM_ServerListNet_StoC_ServerListFailedData;
		}
	};
	//-------------------------------------------------------------------------------------
	//C -> S �O��Client�t�θ��
	struct ServerListNet_CtoS_ClientSystemInfo
	{
		ServerListPacketCommandEnum Command;
		char    Account[ _DEF_MAX_ACCOUNTNAME_SIZE_ ];
		char		MacAddress[_DEF_MAX_MAC_ADDRESS_SIZE_];
		int			DisplayCardVendorID;
		int			DisplayCardDeviceID;
		char		OSINFO[256];
		ServerListNet_CtoS_ClientSystemInfo( )
		{
			Command = EM_ServerListNet_CtoS_ClientSystemInfo;
		}
	};
}
