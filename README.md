# OLC-Font
Extension for the OLC PGE, that handles different fonts

# Exporter
Usage:<br>
The exporter part of the software is a command line program. The following is the template for the command:<br>
OlcFontExporter \<path> [-f #\<color>] [-b #\<color>] [-o \<output>] [-e \<true/false>]<br>
Flags:<br>
<ul>
<li>f Specifies the foreground color</li>
<li>b Specifies the background color</li>
<li>o Specifies the output file</li>
<li>e Specifies if error should be thrown on unknown color</li>
</ul>

By default the output file will be named "output.of"<br>
Color should be given in hexadecimal format with a preceding hashtag in the order of #rrggbbaa

# Loader
A Font object can be created with olc::Font(path-to-file). After this, calling <code>Font#DrawString(x, y, text, color, size);</code> draws the text with the specified font.

The aim of this project is to stay faithful to the original font format found in the pixel game engine.
