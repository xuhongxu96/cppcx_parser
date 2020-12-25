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
                    return null;
                case "data/App.xaml.h - 1":
                    return null;
                case "data/App.xaml.h - 2":
                    return null;
                case "data/App.xaml.cpp - 0":
                    return null;
                default:
                    throw new NotImplementedException();
            }
        }

        [Theory]
        [InlineData("data/App.xaml.h", 0, "void", "f", "")]
        [InlineData("data/App.xaml.h", 1, "void", "g", "int a")]
        [InlineData("data/App.xaml.h", 2, "Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^", "h", "int a", "Platform::StringReference b")]
        [InlineData("data/App.xaml.cpp", 0, "", "", "")]
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
