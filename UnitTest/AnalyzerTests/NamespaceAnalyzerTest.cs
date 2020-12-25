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
        [InlineData("data/App.xaml.h", "Calculator::App,1", "Calculator::App::ApplicationResourceKeys,2")]
        [InlineData("data/App.xaml.cpp", "CalculatorApp,1", "CalculatorApp::ApplicationResourceKeys,1")]
        public void Test(string filename, params string[] namespaces)
        {
            var tu = CxParser.Parse(filename);

            var analyzer = new NamespaceAnalyzer();
            var root = analyzer.Visit(tu);

            foreach (var ns in namespaces)
            {
                var current = root;
                var parts = ns.Split(',');
                var nsParts = parts[0].Trim().Split("::");
                var contextCt = int.Parse(parts[1].Trim());
                foreach (var part in nsParts)
                {
                    Assert.Contains(part, current.NestedNamespaces.Keys);
                    current = current.NestedNamespaces[part];
                }

                Assert.Equal(contextCt, current.Contexts.Count);
            }
        }
    }
}
