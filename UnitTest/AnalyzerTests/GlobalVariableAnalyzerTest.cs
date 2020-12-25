using cppcx.Core.Analyzers;
using cppcx.Core.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnitTest.Utils;
using Xunit;

namespace UnitTest.AnalyzerTests
{
    public class GlobalVariableAnalyzerTest
    {
        private CPPCXParser.SimpleDeclarationContext GetTestGlobalVariableFor(string filename, int option)
        {
            var tu = CxParser.Parse(filename);
            switch ($"{filename} - {option}")
            {
                case "data/App.xaml.h - 0":
                    // extern Platform::StringReference AppMinWindowHeight;
                    return tu.declarationseq().declaration()[0].namespaceDefinition().declarationseq().declaration()[0].namespaceDefinition().declarationseq().declaration()[0].blockDeclaration().simpleDeclaration();
                case "data/App.xaml.h - 1":
                    // int a,b = 3;
                    return tu.declarationseq().declaration()[0].namespaceDefinition().declarationseq().declaration()[0].namespaceDefinition().declarationseq().declaration()[2].blockDeclaration().simpleDeclaration();
                case "data/App.xaml.cpp - 0":
                    // StringReference AppMinWindowHeight(L"AppMinWindowHeight")
                    return tu.declarationseq().declaration()[28].namespaceDefinition().declarationseq().declaration()[0].namespaceDefinition().declarationseq().declaration()[0].blockDeclaration().simpleDeclaration();
                default:
                    throw new NotImplementedException();
            }
        }

        [Theory]
        [InlineData("data/App.xaml.h", 0, "extern Platform::StringReference AppMinWindowHeight")]
        [InlineData("data/App.xaml.h", 1, "int a", "int b = 3")]
        [InlineData("data/App.xaml.cpp", 0, "StringReference AppMinWindowHeight = L\"AppMinWindowHeight\"")]
        public void Test(string filename, int option, params string[] vars)
        {
            var ctx = GetTestGlobalVariableFor(filename, option);
            var analyzer = new GlobalVariableAnalyzer();
            var res = analyzer.Visit(ctx);

            Assert.Equal(vars, res.Select(o => (o.IsExtern ? "extern " : "") + $"{o.Type} {o.Name}" + (o.InitContext == null ? "" : $" = {o.InitContext.GetText().Trim(' ', '=', '(', ')')}")));
        }
    }
}
