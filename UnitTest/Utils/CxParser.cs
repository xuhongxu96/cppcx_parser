using Antlr4.Runtime;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UnitTest.Utils
{
    public static class CxParser
    {
        public static CPPCXParser.TranslationUnitContext Parse(string filename)
        {
            var stream = CharStreams.fromPath(filename);
            var lexer = new CPPCXLexer(stream);
            var tokens = new CommonTokenStream(lexer);
            var parser = new CPPCXParser(tokens);
            parser.BuildParseTree = true;
            return parser.translationUnit();
        }
    }
}
