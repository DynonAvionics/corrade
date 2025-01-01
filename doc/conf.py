DOXYFILE = 'Doxyfile-mcss'

STYLESHEETS = [
    'https://fonts.googleapis.com/css?family=Source+Sans+Pro:400,400i,600,600i%7CSource+Code+Pro:400,400i,600&subset=latin-ext',
    '../css/m-dark+documentation.compiled.css'
]

MAIN_PROJECT_URL = 'https://magnum.graphics/corrade/'

LINKS_NAVBAR2 = [
    (None, 'annotated', []),
    (None, 'files', []),
    ("<a href=\"../../../../magnum-bindings/build/doc/python/index.html\">Python API</a>", [])
]

FINE_PRINT = """<p>Corrade docs. Part of the <a href="https://magnum.graphics/">Magnum project</a>, copyright © <a href="http://mosra.cz/">Vladimír Vondruš</a> and <a href="corrade-credits-contributors.html">contributors</a>, 2007&ndash;2025.<br />Generated by <a href="https://doxygen.org/">Doxygen</a> {doxygen_version} and <a href="https://mcss.mosra.cz/">m.css</a>. Contact the team via <a href="https://github.com/mosra/magnum">GitHub</a>, <a href="https://gitter.im/mosra/magnum">Gitter</a>, <a href="mailto:info@magnum.graphics">e-mail</a> or <a href="https://twitter.com/czmosra">Twitter</a>.</p>"""

FAVICON = 'favicon.ico'

VERSION_LABELS = True

# Code wrapped in DOXYGEN_ELLIPSIS() will get replaced by an (Unicode) ellipsis
# in the output; code wrapped in DOXYGEN_IGNORE() will get replaced by nothing.
# In order to make the same code compilable, add
#
#   #define DOXYGEN_ELLIPSIS(...) __VA_ARGS__
#   #define DOXYGEN_IGNORE(...) __VA_ARGS__
#
# to the snippet code.
def _doxygen_ignore(code: str):
    for macro, replace in [('DOXYGEN_ELLIPSIS(', '…'), ('DOXYGEN_IGNORE(', '')]:
        while macro in code:
            i = code.index(macro)
            depth = 1
            for j in range(i + len(macro), len(code)):
                if code[j] == '(': depth += 1
                elif code[j] == ')': depth -= 1
                if depth == 0: break
            assert depth == 0, "unmatched %s) parentheses in %s" % (macro, code)
            code = code[:i] + replace + code[j+1:]
    return code

M_CODE_FILTERS_PRE = {
    'C++': _doxygen_ignore
}
