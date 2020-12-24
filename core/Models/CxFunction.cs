using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cppcx.Core.Models
{
    public class CxFunction
    {
        public string Name { get; set; }
        public CxType ReturnType { get; set; }
        public List<CxParameter> Parameters { get; init; } = new List<CxParameter>();
        public bool IsStatic { get; set; } = false;
        public CxCode Code { get; set; }
    }
}
