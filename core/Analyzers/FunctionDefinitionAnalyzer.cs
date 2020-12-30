using Antlr4.Runtime.Misc;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cppcx.Core.Analyzers
{
    public class FunctionDefinitionAnalyzer
    {
        public class FunctionDefinition
        {
            public string ClassName { get; set; } = null;
            public string Name { get; set; }
            public string ReturnType { get; set; }
            public List<ParameterAnalyzer.Parameter> Parameters { get; init; } = new List<ParameterAnalyzer.Parameter>();
            public CPPCXParser.StatementSeqContext Statements { get; set; }
        }

        public void Visit([NotNull] CPPCXParser.FunctionDefinitionContext context)
        {
            string retType = null;
            var retTypeAndClassName = context.declSpecifierSeq();
            var className = retTypeAndClassName?.declSpecifier()?.Last()?.GetText();
            if (retTypeAndClassName.ChildCount == 2)
            {
                retType = retTypeAndClassName?.declSpecifier()?.First()?.GetText();
            }

            var name = context.declarator().GetText().Trim(':');
            throw new NotImplementedException();
        }
    }
}
