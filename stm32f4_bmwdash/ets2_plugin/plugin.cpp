/*****************************************************************************************************/
/*                                                                                                   */
/*  Az instrument_comport.txt fajlt es a leforditott DLL-t a jatek plugins mappajaba kell masolni.   */
/*  A txt fajl csak egyetlen szamot tartalmazhat, pl. COM15 eseten a fajl tartalma 15.               */
/*                                                                                                   */
/*****************************************************************************************************/


#include <sstream>
#include <fstream>
#include <iomanip>
#include <assert.h>
#define WINVER         0x0500
#define _WIN32_WINNT   0x0500
#include <windows.h>
#include "scssdk_telemetry.h"
#include "eurotrucks2/scssdk_telemetry_eut2.h"


typedef struct{
	float speed;
	float engine_rpm;
	bool light_lblinker;
	bool light_rblinker;
	bool light_high_beam;
} telemetry_t;


static telemetry_t telemetry;
static HANDLE serial_port;


SCSAPI_VOID telemetry_frame_end(const scs_event_t event, const void *const event_info, const scs_context_t context){
	static bool prevLSignal=false, prevRSignal=false, prevHighBeam=false;
	static unsigned long prevTime=0;
	if(serial_port==INVALID_HANDLE_VALUE) return;
	/* idozites */
	unsigned long now=GetTickCount();
	if((now-prevTime)<50) return;
	prevTime=now;
	/* adatok szamitasa */
	int rpm=(int)(telemetry.engine_rpm*2); //*2, hogy jobban ki legyen vezerelve a muszer
	int speed=abs((int)(telemetry.speed*3.6)); //m/s --> km/h
	int lSignal=0;
	if(telemetry.light_lblinker && !prevLSignal) //bal index be
		lSignal=1;
	else if(!telemetry.light_lblinker && prevLSignal) //bal index ki
		lSignal=2;
	int rSignal=0;
	if(telemetry.light_rblinker && !prevRSignal) //jobb index be
		rSignal=1;
	else if(!telemetry.light_rblinker && prevRSignal) //jobb index ki
		rSignal=2;
	int highBeam=0;
	if(telemetry.light_high_beam && !prevHighBeam) //fenyszoro be
		highBeam=1;
	else if(!telemetry.light_high_beam && prevHighBeam) //fenyszoro ki
		highBeam=2;
	prevLSignal=telemetry.light_lblinker;
	prevRSignal=telemetry.light_rblinker;
	prevHighBeam=telemetry.light_high_beam;
	/* uzenet osszeallitasa */
	std::ostringstream ss;
	ss << "$"; //start delimiter
	ss << std::setw(4) << std::setfill('0') << rpm; //fordulatszam xxxx formatumban
	ss << " "; //szunet
	ss << std::setw(3) << std::setfill('0') << speed; //sebesseg xxx formatumban
	ss << " "; //szunet
	ss << std::setw(1) << lSignal; //bal index
	ss << " "; //szunet
	ss << std::setw(1) << rSignal; //jobb index
	ss << " "; //szunet
	ss << std::setw(1) << highBeam; //fenyszoro
	ss << "|\n"; //end delimiter
	/* uzenet kuldese */
	char buffer[17];
	ss.str().copy(buffer,17);
	for(unsigned i=0;i<17;i++){
		unsigned long bytesWritten;
		WriteFile(serial_port,&buffer[i],1,&bytesWritten,NULL);
		FlushFileBuffers(serial_port);
	}
}

SCSAPI_VOID telemetry_store_float(const scs_string_t name, const scs_u32_t index, const scs_value_t *const value, const scs_context_t context){
	assert(value);
	assert(value->type==SCS_VALUE_TYPE_float);
	assert(context);
	*static_cast<float *>(context)=value->value_float.value;
}

SCSAPI_VOID telemetry_store_bool(const scs_string_t name, const scs_u32_t index, const scs_value_t *const value, const scs_context_t context){
	assert(value);
	assert(value->type==SCS_VALUE_TYPE_bool);
	assert(context);
	*static_cast<bool *>(context)=(value->value_bool.value!=0);
}

SCSAPI_RESULT scs_telemetry_init(const scs_u32_t version, const scs_telemetry_init_params_t *const params){
	const scs_telemetry_init_params_v100_t* const version_params=static_cast<const scs_telemetry_init_params_v100_t *>(params);
	/* fajl beolvasasa */
	std::string dir;
	char str[256];
	GetCurrentDirectoryA(256,str);
	dir.assign(str);
	std::ifstream file(dir+"\\plugins\\instrument_comport.txt");
	int n;
	file>>n;
	file.close();
	std::ostringstream port;
	port<<"//./COM"<<n;
	/* COM port megnyitasa */
	serial_port=CreateFileA(port.str().c_str(),GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
	/* callback fuggvenyek regisztralasa */
	version_params->register_for_event(SCS_TELEMETRY_EVENT_frame_end,telemetry_frame_end,NULL);
	version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_speed,SCS_U32_NIL,SCS_VALUE_TYPE_float,SCS_TELEMETRY_CHANNEL_FLAG_none,telemetry_store_float,&telemetry.speed);
	version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_engine_rpm,SCS_U32_NIL,SCS_VALUE_TYPE_float,SCS_TELEMETRY_CHANNEL_FLAG_none,telemetry_store_float,&telemetry.engine_rpm);
	version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_light_lblinker,SCS_U32_NIL,SCS_VALUE_TYPE_bool,SCS_TELEMETRY_CHANNEL_FLAG_none,telemetry_store_bool,&telemetry.light_lblinker);
	version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_light_rblinker,SCS_U32_NIL,SCS_VALUE_TYPE_bool,SCS_TELEMETRY_CHANNEL_FLAG_none,telemetry_store_bool,&telemetry.light_rblinker);
	version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_light_high_beam,SCS_U32_NIL,SCS_VALUE_TYPE_bool,SCS_TELEMETRY_CHANNEL_FLAG_none,telemetry_store_bool,&telemetry.light_high_beam);
	return SCS_RESULT_ok;
}

SCSAPI_VOID scs_telemetry_shutdown(void){
	CloseHandle(serial_port);
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason_for_call, LPVOID reseved){
	if(reason_for_call==DLL_PROCESS_DETACH) CloseHandle(serial_port);
	return TRUE;
}
