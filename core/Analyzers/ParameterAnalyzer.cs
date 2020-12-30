using Antlr4.Runtime.Misc;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cppcx.Core.Analyzers
{
    public class ParameterAnalyzer
    {
        public class Parameter
        {
            public string Type { get; set; }
            public string Name { get; set; }
            public string DefaultValue { get; set; }
        }

        private Parameter VisitParameterDeclaration([NotNull] CPPCXParser.ParameterDeclarationContext context)
        {
            return new Parameter
            {
                Type = context.declSpecifierSeq().declSpecifier()[0].GetText(),
                Name = context.declSpecifierSeq().declSpecifier()[1].GetText().Trim('*', '^', '&'),
            };
        }

        public List<Parameter> Visit([NotNull] IEnumerable<CPPCXParser.ParameterDeclarationContext> contexts)
        {
            var res = new List<Parameter>();
            foreach (var parameterDecl in contexts)
            {
                res.Add(VisitParameterDeclaration(parameterDecl));
            }

            return res;
        }
    }
}
