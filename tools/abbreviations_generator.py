
import sys
import os
OVERRIDE = True
#python3.10 ../tools/abbreviations_generator.py abbr.txt ../include/google-translate/abbr.hpp ../src/abbr.cpp
def main():
    if (len(sys.argv) >= 4):
        source_dir = sys.argv[1]
        dest_dir = sys.argv[2]
        dest_src_dir = sys.argv[3]
        if not OVERRIDE and (os.path.exists(dest_dir)):
            return
    else:
        return

    LANG = [ "Auto",
                "Afrikaans",
                "Albanian",
                "Amharic",
                "Arabic",
                "Armenian",
                "Assamese",
                "Aymara",
                "Azerbaijani",
                "Bambara",
                "Basque",
                "Belarusian",
                "Bengali",
                "Bhojpuri",
                "Bosnian",
                "Bulgarian",
                "Catalan",
                "Cebuano",
                "Chichewa",
                "Chinese_Simplified",
                "Chinese_Traditional",
                "Corsican",
                "Croatian",
                "Czech",
                "Danish",
                "Dhivehi",
                "Dogri",
                "Dutch",
                "English",
                "Esperanto",
                "Estonian",
                "Ewe",
                "Filipino",
                "Finnish",
                "French",
                "Frisian",
                "Galician",
                "Georgian",
                "German",
                "Greek",
                "Guarani",
                "Gujarati",
                "Haitian_Creole",
                "Hausa",
                "Hawaiian",
                "Hebrew",
                "Hindi",
                "Hmong",
                "Hungarian",
                "Icelandic",
                "Igbo",
                "Ilocano",
                "Indonesian",
                "Irish",
                "Italian",
                "Japanese",
                "Javanese",
                "Kannada",
                "Kazakh",
                "Khmer",
                "Kinyarwanda",
                "Konkani",
                "Korean",
                "Krio",
                "Kurdish_Kurmanji",
                "Kurdish_Sorani",
                "Kyrgyz",
                "Lao",
                "Latin",
                "Latvian",
                "Lingala",
                "Lithuanian",
                "Luganda",
                "Luxembourgish",
                "Macedonian",
                "Maithili",
                "Malagasy",
                "Malay",
                "Malayalam",
                "Maltese",
                "Maori",
                "Marathi",
                "Meiteilon_Manipuri",
                "Mizo",
                "Mongolian",
                "Myanmar_Burmese",
                "Nepali",
                "Norwegian",
                "Odia_Oriya",
                "Oromo",
                "Pashto",
                "Persian",
                "Polish",
                "Portuguese",
                "Punjabi",
                "Quechua",
                "Romanian",
                "Russian",
                "Samoan",
                "Sanskrit",
                "Scots_Gaelic",
                "Sepedi",
                "Serbian",
                "Sesotho",
                "Shona",
                "Sindhi",
                "Sinhala",
                "Slovak",
                "Slovenian",
                "Somali",
                "Spanish",
                "Sundanese",
                "Swahili",
                "Swedish",
                "Tajik",
                "Tamil",
                "Tatar",
                "Telugu",
                "Thai",
                "Tigrinya",
                "Tsonga",
                "Turkish",
                "Turkmen",
                "Twi",
                "Ukrainian",
                "Urdu",
                "Uyghur",
                "Uzbek",
                "Vietnamese",
                "Welsh",
                "Xhosa",
                "Yiddish",
                "Yoruba",
                "Zulu"
    ]

    D = {}

    D["Danish"] = "Æ Ø Å pp etc adm afd ang anm arr aud aut cit d e eks ell etc f fmd forb gns gr h hr i ift iht indb ital jf jur kld komm komp kr kvt l m maks mdl mdr mfl mht n nr nydann obs omkr opr osv p pga phil prof præs pæd resp s sept sml str tlf u udd v vejl vha vol årh".split(' ')
    D["Dutch"] = "St no etc fig al cf bc c.i dr drs ds fa fam ing ir jhr jkvr jr Mej Mw prof Z.Em Z.H Z.K.H Z.M a.g.v bijv d.w.z e.c e.g e.k ev i.p.v i.t.t m.a.w m.b.t m.i m.i.v nrs nr".split(' ')
    D["English"] = "Mr Mrs No pp St Sr Jr Bros etc vs Fig Jan Feb Mar Apr Jun Jul Aug Sep Sept Oct Nov Dec Inc Ms Gen Sen Prof Dr Corp Co Adm Bros Capt Col Corp Dr Gen Gov Lt Maj Messrs Mr Mrs Ms Ph Prof Rep Reps Rev".split(' ')
    D["Finnish"] = "Mr St Jr vs Fig Inc Corp a eKr esim jne joht mrd n p ts ym yms".split(' ')
    D["French"] = "Mr No St Sr Jr Mar Sept Oct Nov Dec Ph.D Inc Ms Dr Co A.M B.P e.g E.V i.e L.D N.B N.S N.D p.ex P.S R.A.S R.P S.S S.A S.A.R S.A.S S.E S.M Z.I".split(' ')
    D["German"] = "Mr Mrs pp St no Sr Jr Bros etc vs Fig Jan Feb Mar Apr Jun Jul Aug Sept Oct Nov cf Inc Ms Gen Sen Prof IV VII VII XIV Adj Adv Bldg Brig bros Col DR Gen Hon Hosp Insp MM Maj Messrs Mlle Mme Mr Mrs Ms Msgr Ord Ph Prof Rep Res Rev Rt Sen Sens Sgt Sr St Supt Mio Mrd bzw vs usw d.h z.B u.a etc Mrd MwSt ggf D.h m.E Art Nr pp ca Ca bzgl vgl z.T ff u.E Buchst sog Std evtl Zt Chr u.U o.ä dgl dergl Co zzt usf".split(' ')
    D["Greek"] = "Α Β Γ Δ Ε Ζ Η Θ Ι Κ Λ Μ Ν Ξ Ο Π Ρ Σ Τ Υ Φ Χ Ψ Ω Αν Απρ Απ Αριθ Γεν Γλ Κορ Ντ Ξεν Πρξ Πρωτ αγγλ ανατ αντ αριθμ γερμ γεωγρ γραμ δηλ δολ δρχ δ εικ εκατ εκδ εκ κ λατ μτφρ οικ ονομ παραγ πβ περιλ περ πολ πρβλ πρβ πρλ προφ π ρ σσ στ συγκρ συμπερ σ τουρκ τροποπ τροπ υποθ υπ χλγ χλμ χρ".split(' ')
    D["Hungarian"] = "Mr Mrs No St Sr Jr Bros Jan Feb Mar Apr Aug Oct Okt Nov Dec Ph.D PhD Inc Ms Gen Prof Dr Corp Co".split(' ')
    D["Italian"] = "Mr Mrs No St Sr Jr Bros Fig Jan Feb Mar Apr Aug Oct Nov Dec Ph.D Inc Ms Gen Sen Prof Dr Corp Co Arch Co Dott Dr Id Ing Mons p.es Prof Rev Rif Sig Soc".split(' ')
    D["Norwegian"] = "Mr Mrs pp St etc Fig Inc Dr dvs nr ang g jf kgl m s b d f g k konf l m n s t".split(' ')
    D["Romanian"] = "Î Mr Mrs No pp Sr Jr Prof Dr Sf dvs sil conj p pg pp vol".split(' ')
    D["Russian"] = "А Б В Г Д Е Ё Ж З И Й К Л М Н О П Р С Т У Ф Х Ц Ч Ш Щ Ъ Ы Ь Э Ю Я см стр".split(' ')
    D["Spanish"] = "Mr Mrs No St Sr Jr Bros Fig Jan Feb Mar Apr Jun Jul Aug Sep Oct Nov Dec Inc Ms Gen Prof Dr Corp Co".split(' ')
    D["Swedish"] = "Ä Ö Ü etc Fig fig Prof adv bibl d dial etc exkl f omkr plur t e f g m omkr s utg".split(' ')

    L = []
    for k in D:
        for i in D[k]:
            c = chr(LANG.index(k))
            if c == '"':
                s = f"\"\\\"{i}\""
            elif c < '!':
                #s = f"\"\\{ord(c):03d}{i}\""
                x = ''.join([f"\\x{ord(j):02X}" for j in i])
                s = f"\"\\x{ord(c):02X}{x}\""
            else:
                s = c+i
            if s not in L:
                L.append(s)#(c if c >= ' ' else f'\\x{ord(c):02X}')
    

    f = open(f"{source_dir}", 'w')

    s = '\n'.join(L)
    f.write(
f"""
%{"{"}
#pragma once
#include <string>
%{"}"}
%%
{s}
%%
inline bool abbreviations::match(const char * str, int language) {"{"}
  std::string s;
  s += (char)language;
  s += str;
  return bool(abbreviations::find(s.c_str(), s.size()));
{"}"}
"""
)

    f.close()

    from subprocess import run
    run([
        "gperf",
            f"{source_dir}",
            "--language=C++",
            "--class-name=abbreviations",
            "--lookup-function-name=find",
            "--includes",
            f"--output-file={dest_dir}"
        ]
    )

    f = open(f"{dest_dir}", 'r')
    s = f.read().replace("register ","").replace("  ", "    ")
    f.close()
        
    #print(s[funcEnd:funcEnd+10])
    i = s.find("};")
    s = s[:i] + """    static inline bool match(const char * str, int language);
""" + s[i:]

    i = s.find("#define")
    r = s.find('\n', s.rfind("#define"))


    macros = s[i:r]
    s = s[:i] + s[r:]
    print()
    


    func = """const char *
abbreviations::find (const char *str, unsigned int len)
{"""
    funcLoc = s.find(func)
    funcEnd = funcLoc + len(func)
    depth = 1
    while depth and funcEnd < len(s):
        if (s[funcEnd] == "{"):
            depth += 1
        elif (s[funcEnd] == "}"):
            depth -= 1
        elif (s[funcEnd] == '"'):
            while (s[funcEnd] != '"' and funcEnd < len(s)):
                if (s[funcEnd] == '\\'):
                    funcEnd += 1
                funcEnd += 1
        elif (s[funcEnd] == '\''):
            while (s[funcEnd] != '\'' and funcEnd < len(s)):
                funcEnd += 1
        funcEnd += 1
    f.close()
    f = open(f"{dest_dir}", 'w')
    f.write(s[:funcLoc] + s[funcEnd:])
    f.close()

    f = open(f"{dest_src_dir}", 'w')
    f.write("""
#include "abbr.hpp"
"""+macros+'\n'+s[funcLoc:funcEnd])


if __name__ == "__main__":
    main()