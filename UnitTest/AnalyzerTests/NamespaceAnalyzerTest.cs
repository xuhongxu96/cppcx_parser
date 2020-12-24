using Antlr4.Runtime.Tree;
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
    public class NamespaceAnalyzerTest
    {
        [Theory]
        [InlineData("data/App.xaml.h", "Calculator::App", "Calculator::App::ApplicationResourceKeys")]
        [InlineData("data/App.xaml.cpp", "CalculatorApp::ApplicationResourceKeys")]
        public void Test(string filename, params string[] namespaces)
        {
            var tu = CxParser.Parse(filename);

            var analyzer = new NamespaceAnalyzer();
            ParseTreeWalker.Default.Walk(analyzer, tu);

            foreach (var ns in namespaces)
            {
                var current = analyzer.Root;
                var nsParts = ns.Split("::");
                foreach (var part in nsParts)
                {
                    Assert.Contains(part, current.NestedNamespaces.Keys);
                    current = current.NestedNamespaces[part];
                }

                Assert.NotNull(current.Context);
            }
        }
    }
}
