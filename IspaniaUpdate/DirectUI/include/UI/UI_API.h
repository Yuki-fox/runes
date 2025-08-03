#ifndef MING_UI_API_H
#define MING_UI_API_H

#ifdef DIRECTUI_EXPORTS
	#define UILIB_API __declspec(dllexport)
#else
	#ifdef DIRECTUI_USRDLLX
		#define UILIB_API __declspec(dllimport)

	#else 
		#define UILIB_API
	#endif
#endif

#endif //MING_UI_API_H
