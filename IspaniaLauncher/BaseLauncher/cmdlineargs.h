#ifndef CMDLINEARGS_H
#define CMDLINEARGS_H

#include <windows.h>
#include <vector>

bool IsSpace(int C)
{

	if(C==0x20 || C=='\t'|| C==0)
		return true;

	return false;
}

class CmdLineArgs : public std::vector<char*>
{
public:
    CmdLineArgs ()
    {
        // Save local copy of the command line string, because
        // ParseCmdLine() modifies this string while parsing it.
        PSZ cmdline = GetCommandLine();
		int len=strlen (cmdline);
        m_cmdline = new char [ len+ 1];
        if (m_cmdline)
        {
            strcpy (m_cmdline, cmdline);
            ParseCmdLine(); 
        }
    }
    ~CmdLineArgs()
    {
        delete []m_cmdline;
    }

private:
    PSZ m_cmdline; // the command line string

    ////////////////////////////////////////////////////////////////////////////////
    // Parse m_cmdline into individual tokens, which are delimited by spaces. If a
    // token begins with a quote, then that token is terminated by the next quote
    // followed immediately by a space or terminator.  This allows tokens to contain
    // spaces.
    // This input string:     This "is" a ""test"" "of the parsing" alg"o"rithm.
    // Produces these tokens: This, is, a, "test", of the parsing, alg"o"rithm
    ////////////////////////////////////////////////////////////////////////////////
    void ParseCmdLine ()
    {
        enum { TERM  = '\0',
               QUOTE = '\"' };

        bool bInQuotes = false;
        PSZ pargs = m_cmdline;

        while (*pargs)
        {
            while (IsSpace (*pargs))        // skip leading whitespace
                pargs++;

            bInQuotes = (*pargs == QUOTE);  // see if this token is quoted

            if (bInQuotes)                  // skip leading quote
                pargs++; 

            push_back (pargs);              // store position of current token

            // Find next token.
            // NOTE: Args are normally terminated by whitespace, unless the
            // arg is quoted.  That's why we handle the two cases separately,
            // even though they are very similar.
            if (bInQuotes)
            {
				int c=*pargs;
				int cnext=pargs[1];
				
                // find next quote followed by a space or terminator
                while (c && 
                      !(c == QUOTE && (IsSpace (cnext) || cnext == TERM)))
				{
                    pargs++;
					c=*pargs;
					 cnext=pargs[1];
				}
                if (*pargs)
                {
                    *pargs = TERM;  // terminate token
                    if (pargs[1])   // if quoted token not followed by a terminator
                        pargs += 2; // advance to next token
                }
            }
            else
            {
                // skip to next non-whitespace character
                while (*pargs && !IsSpace (*pargs)) 
                    pargs++;
                if (*pargs && IsSpace (*pargs)) // end of token
                {
                   *pargs = TERM;    // terminate token
                    pargs++;         // advance to next token or terminator
                }
            }
        } // while (*pargs)
    } // ParseCmdLine()
}; // class CmdLineArgs


#endif // CMDLINEARGS_H