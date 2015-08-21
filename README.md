Dies ist der Quellcode für sowohl Controller (Verzeichnis "uC") als auch PC-Tools (Verzeichnis "PC").

Der uC-Code ist zuletzt mit WinAVR 2007_01_22 kompiliert worden. 
Dazu sind noch ein paar Anpassungen nötig, die sich im "addon"-Verzeichnis befinden.
Sie betreffen die Unterstützung des Compilerschalters "-mint8", der den Code kleiner macht.
Für den Bootloader ist dieser Schalter zwingend nötig, beim eigentlichen Code
auch ganz nützlich, daher wird er in allen uC-Projekten verwendet.
Der Code ist für den Fujitsu-Controller schon eine Weile nicht mehr nachgepflegt worden,
gut möglich daß der so nicht mehr komiliert. (Ist aber zunehmend unwichtig.)

Ferner befindet sich im "addon"-Verzeichnis noch ein Perl-Script und ein Batchfile,
die sind nützlich falls man MS Developer Studio als IDE verwendet.
Damit werden die Fehlermeldungen von gcc in das MS-Format umformatiert,
dann kommt man mit einem Doppelklick auf die Fehlermeldung in die entsprechende Codezeile.
Die beiden Dateien müssen in einem Pfad ohne Leerzeichen sein, der auch im
Developer Studio unter Tools.Options.Directories.Executable-files eingerichtet ist.
Ferner muß auf dem PC Perl installiert sein, z.B. ActiveState Perl.
