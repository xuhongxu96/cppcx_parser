using cppcx.Core.Analyzers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnitTest.Utils;
using Xunit;

namespace UnitTest.AnalyzerTests
{
    public class FunctionDeclarationAnalyzerTest
    {
        private CPPCXParser.SimpleDeclarationContext GetTestFunctionDeclarationFor(string filename, int option)
        {
            var tu = CxParser.Parse(filename);
            switch ($"{filename} - {option}")
            {
                case "data/App.xaml.h - 0":
                    // void f();
                    return tu.declarationseq().declaration()[0].namespaceDefinition().declarationseq().declaration()[0].namespaceDefinition().declarationseq().declaration()[3].blockDeclaration().simpleDeclaration();
                case "data/App.xaml.h - 1":
                    // void g(int a);
                    return tu.declarationseq().declaration()[0].namespaceDefinition().declarationseq().declaration()[0].namespaceDefinition().declarationseq().declaration()[4].blockDeclaration().simpleDeclaration();
                case "data/App.xaml.h - 2":
                    return tu.declarationseq().declaration()[0].namespaceDefinition().declarationseq().declaration()[1].namespaceDefinition().declarationseq().declaration()[0].blockDeclaration().simpleDeclaration();
                default:
                    throw new NotImplementedException();
            }
        }

        [Theory]
        [InlineData("data/App.xaml.h", 0, "void", "f")]
        [InlineData("data/App.xaml.h", 1, "void", "g", "int a")]
        [InlineData("data/App.xaml.h", 2, "Windows::ApplicationModel::Activation::LaunchActivatedEventArgs", "h", "int a", "Platform::StringReference b")]
        public void Test(string filename, int option, string retType, string funcName, params string[] parameters)
        {
            var ctx = GetTestFunctionDeclarationFor(filename, option);
            var analyzer = new FunctionDeclarationAnalyzer();
            var res = analyzer.Visit(ctx);

            Assert.Equal(retType, res.ReturnType);
            Assert.Equal(funcName, res.Name);
            Assert.Equal(parameters, res.Parameters.Select(o => $"{o.Type} {o.Name}" + (o.DefaultValue == null ? "" : $" = {o.DefaultValue}")));
        }
    }
}
