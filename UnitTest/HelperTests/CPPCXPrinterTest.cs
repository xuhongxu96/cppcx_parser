using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using System;
using Xunit;
using Xunit.Abstractions;
using cppcx.Core.Helpers;
using UnitTest.Utils;

namespace UnitTest.HelperTests
{
    public class CPPCXPrinterTest
    {
        private readonly ITestOutputHelper _output;

        public CPPCXPrinterTest(ITestOutputHelper output)
        {
            _output = output;
        }

        [Theory]
        [InlineData("data/App.xaml.h")]
        [InlineData("data/App.xaml.cpp")]
        public void TestParseFile(string filename)
        {
            var tu = CxParser.Parse(filename);

            var writer = new CPPCXPrinter();
            ParseTreeWalker.Default.Walk(writer, tu);
            _output.WriteLine(writer.Content);
        }
    }
}
