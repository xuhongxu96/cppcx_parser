using Antlr4.Runtime.Tree;
using cppcx.Core.Analyzers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnitTest.Utils;
using Xunit;
using Xunit.Abstractions;

namespace UnitTest.AnalyzerTests
{
    public class NamespaceBodyAnalyzerTest
    {
        private readonly ITestOutputHelper _output;

        public NamespaceBodyAnalyzerTest(ITestOutputHelper output)
        {
            _output = output;
        }

        private CPPCXParser.DeclarationseqContext GetTestNamespaceFor(string filename, int option)
        {
            var tu = CxParser.Parse(filename);
            switch ($"{filename} - {option}")
            {
                case "data/App.xaml.h - 0":
                    // Calculator::App
                    return tu.declarationseq().declaration()[0].namespaceDefinition().declarationseq();
                case "data/App.xaml.h - 1":
                    // Calculator::App::ApplicationResourceKeys
                    return tu.declarationseq().declaration()[0].namespaceDefinition().declarationseq().declaration()[0].namespaceDefinition().declarationseq();
                case "data/App.xaml.cpp - 0":
                    return tu.declarationseq();
                default:
                    throw new NotImplementedException();
            }
        }

        [Theory]
        [InlineData("data/App.xaml.h", 0, 0, 1)]
        [InlineData("data/App.xaml.h", 1, 3, 0, 2)]
        [InlineData("data/App.xaml.cpp", 0, 0, 0, 0, 16)]
        public void Test(string filename, int option, int expectedVarCount = 0, int expectedClassCount = 0, int expectedFuncDeclCount = 0, int expectedFuncDefCount = 0)
        {
            var analyzer = new NamespaceBodyAnalyzer();
            var ns = GetTestNamespaceFor(filename, option);

            var result = analyzer.Visit(ns);

            _output.WriteLine(result.ToString());

            Assert.Equal(expectedVarCount, result.GlobalVariables.Count);
            Assert.Equal(expectedClassCount, result.Classes.Count);
            Assert.Equal(expectedFuncDeclCount, result.FunctionDeclarations.Count);
            Assert.Equal(expectedFuncDefCount, result.FunctionDefinitions.Count);
        }
    }
}
